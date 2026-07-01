#include <ncurses.h>
#include <math.h>
#include "../include/simulation.h"

float CalculateEccentricAnomaly(double mna, float ecc) {
    // use newton-raphson to approximate E.
    // f(x) = E - e*sin E - M
    // f'(x) = 1 - e*cos E
    float E = mna;

    for (int i = 0; i < 10; i++) {
        float f = E - ecc*sinf(E)-mna;
        float fPrime = 1 - ecc*cosf(E);

        E -= f/fPrime;
    }
    
    return E;
}

FPoint3 GetPointOnElipse(float xLocal, float yLocal, float trigArr[6]) {
    float sinOmega = trigArr[0]; float cosOmega = trigArr[1];
    float sinLan = trigArr[2]; float cosLan = trigArr[3];
    float sinInc = trigArr[4]; float cosInc = trigArr[5];

    float x = xLocal * (cosOmega * cosLan - sinOmega * sinLan * cosInc) - yLocal * (sinOmega * cosLan + cosOmega * sinLan * cosInc);
    float y = xLocal * (cosOmega * sinLan + sinOmega * cosLan * cosInc) - yLocal * (sinOmega * sinLan - cosOmega * cosLan * cosInc);
    float z = xLocal * (sinOmega * sinInc)                                    + yLocal * (cosOmega * sinInc);

    return (FPoint3){x, y, z};
}

void RenderOrbit(Planet planet, Point center) {
    OrbitParams orbit = planet.orbitparams;
    float ecc = orbit.eccentricity;

    // build trig array
    double lan = orbit.lan;
    double omega = orbit.lpe - lan;
    float i = orbit.inclination;
    float trigArr[6] = {sinf(omega), cosf(omega), sinf(lan), cosf(lan), sinf(i), cosf(i)};

    int a = orbit.smaxis;
    int b = (int)(sqrt(1-ecc*ecc)*a); // semi minor axis

    int max = (a > b) ? a : b;
    float step = 0.5/max;

    int xc = center.x;
    int yc = center.y;

    // render elipse
    for (float theta = 0.0; theta < M_TAU; theta += step) {
        float xLocal = a * (cosf(theta) - ecc);
        float yLocal = b * sinf(theta);
        
        FPoint3 point = GetPointOnElipse(xLocal, yLocal, trigArr);
        float camY = point.y * sin(VIEW_RAD) - point.z * cos(VIEW_RAD);
        
        mvaddch(yc-camY, xc+point.x, '.' | COLOR_PAIR(planet.color.colorID));
    }
}

void RenderPlanet(Planet planet, Point center) {
    OrbitParams orbit = planet.orbitparams;

    float ecc = orbit.eccentricity;

    // build trig array
    double lan = orbit.lan;
    double omega = orbit.lpe - lan;
    float i = orbit.inclination;
    float trigArr[6] = {sinf(omega), cosf(omega), sinf(lan), cosf(lan), sinf(i), cosf(i)};
    
    int a = orbit.smaxis;
    int b = (int)(sqrt(1-ecc*ecc)*a); // semi minor axis

    int xc = center.x;
    int yc = center.y;

    // render planet via keplers equation
    float E = CalculateEccentricAnomaly(orbit.mna, ecc);
    int planetXLocal = a * (cosf(E) - ecc);
    int planetYLocal = b * sinf(E);
    
    FPoint3 planetPos = GetPointOnElipse(planetXLocal, planetYLocal, trigArr);
    float camY = planetPos.y * sin(VIEW_RAD) - planetPos.z * cos(VIEW_RAD);

    // render planet
    mvaddch(yc-camY, xc+planetPos.x, 'O' | COLOR_PAIR(planet.color.colorID)); 

    // render name
    char* name = planet.name;
    attron(COLOR_PAIR(planet.color.colorID));
    mvaddstr(yc-camY-1, xc+planetPos.x+2, name);
    attroff(COLOR_PAIR(planet.color.colorID));
}

void RenderScene(Scene scene) {
    Point center = scene.center;
    
    // render planet orbits
    for (int i = 0; i < scene.planetCount; i++) {
        RenderOrbit(scene.planets[i], center);
    }

    // render planets and their names
    for (int i = 0; i < scene.planetCount; i++) {
        RenderPlanet(scene.planets[i], center);
    }

    mvaddch(center.y, center.x, '*');
}
