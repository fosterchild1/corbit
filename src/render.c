#include <ncurses.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../include/simulation.h"

int8_t* depthBuf;

float CalculateEccentricAnomaly(double mna, float ecc) {
    // use newton-raphson to approximate E.
    // f(x) = E - e*sin E - M
    // f'(x) = 1 - e*cos E
    float E = mna;

    for (int i = 0; i < 5; i++) {
        float f = E - ecc * sinf(E) - mna;
        float fPrime = 1 - ecc * cosf(E);

        E -= f/fPrime;
    }
    
    return E;
}

FPoint3 GetPointOnElipse(float xLocal, float yLocal, float trigArr[6]) {
    float sinOmega = trigArr[0]; float cosOmega = trigArr[1];
    float sinLan = trigArr[2]; float cosLan = trigArr[3];
    float sinInc = trigArr[4]; float cosInc = trigArr[5];

    // thanks superchil for this monstrosity
    float x = xLocal * (cosOmega * cosLan - sinOmega * sinLan * cosInc) - yLocal * (sinOmega * cosLan + cosOmega * sinLan * cosInc);
    float y = xLocal * (cosOmega * sinLan + sinOmega * cosLan * cosInc) - yLocal * (sinOmega * sinLan - cosOmega * cosLan * cosInc);
    float z = xLocal * (sinOmega * sinInc)                                    + yLocal * (cosOmega * sinInc);

    return (FPoint3){x, y, z};
}

void RenderOrbit(Planet planet, Camera camera, Point center) {
    OrbitParams orbit = planet.orbitparams;
    float ecc = orbit.eccentricity;
    float viewRad = camera.viewAngle * M_PI/180.0;

    // build trig array
    double lan = orbit.lan;
    double omega = orbit.lpe - lan;
    float i = orbit.inclination;
    float trigArr[6] = {sinf(omega), cosf(omega), sinf(lan), cosf(lan), sinf(i), cosf(i)};

    int a = orbit.smaxis;
    int b = (int)(sqrt(1 - ecc*ecc) * a); // semi minor axis

    int max = (a > b) ? a : b;
    float step = 0.5/max;

    int xc = center.x;
    int yc = center.y;

    // render elipse
    int lastY = 0; int lastX = 0;
    for (float theta = 0.0; theta < M_TAU; theta += step) {
        float xLocal = a * (cosf(theta) - ecc);
        float yLocal = b * sinf(theta);
        
        FPoint3 point = GetPointOnElipse(xLocal, yLocal, trigArr);

        float camY = point.y * sin(viewRad) - point.z * cos(viewRad);
        int targetY = yc-camY; int targetX = xc+point.x;
        if (targetY == lastY && targetX == lastX) continue;

        // get depth
        int8_t depth = point.y * cos(viewRad) + point.z * sin(viewRad);
        int depthIdx = (targetY * xc * 2) + targetX;
        if (depthBuf[depthIdx] < depth) continue;
        
        lastY = targetY; lastX = targetX;
        mvaddch(targetY, targetX, '.' | COLOR_PAIR(planet.color.colorID));
    }
}

void RenderPlanet(Planet planet, Camera camera, Point center) {
    OrbitParams orbit = planet.orbitparams;
    float ecc = orbit.eccentricity;
    float viewRad = camera.viewAngle * M_PI/180.0;

    // build trig array
    double lan = orbit.lan;
    double omega = orbit.lpe - lan;
    float i = orbit.inclination;
    float trigArr[6] = {sinf(omega), cosf(omega), sinf(lan), cosf(lan), sinf(i), cosf(i)};
    
    int a = orbit.smaxis;
    int b = (int)(sqrt(1 - ecc*ecc) * a); // semi minor axis

    int xc = center.x;
    int yc = center.y;

    // render planet via keplers equation
    float E = CalculateEccentricAnomaly(orbit.mna, ecc);
    float planetXLocal = a * (cosf(E) - ecc);
    float planetYLocal = b * sinf(E);
    
    FPoint3 planetPos = GetPointOnElipse(planetXLocal, planetYLocal, trigArr);
    float camY = planetPos.y * sin(viewRad) - planetPos.z * cos(viewRad);

    // render planet
    mvaddch(yc-camY, xc+planetPos.x, 'O' | COLOR_PAIR(planet.color.colorID)); 

    // render name
    char* name = planet.name;
    attron(COLOR_PAIR(planet.color.colorID));
    mvaddstr(yc-camY-1, xc+planetPos.x+2, name);
    attroff(COLOR_PAIR(planet.color.colorID));
}

void RenderScene(Scene scene) {
    // create depth buffer
    if (depthBuf == NULL) {
        int bufSize = scene.center.x * scene.center.y * 4;
        depthBuf = (int8_t*)malloc(bufSize * sizeof(int8_t));
        if (depthBuf == NULL) exit(0);
        
        memset(depthBuf, INT8_MAX, bufSize);
    }
    
    Point center = scene.center;
    
    // render planet orbits
    for (int i = 0; i < scene.planetCount; i++) {
        RenderOrbit(scene.planets[i], scene.camera, center);
    }

    // render planets and their names
    for (int i = 0; i < scene.planetCount; i++) {
        RenderPlanet(scene.planets[i], scene.camera, center);
    }

    mvaddch(center.y, center.x, '*');
}
