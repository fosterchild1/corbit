#include <ncurses.h>
#include <math.h>
#include "../include/simulation.h"

float CalculateEccentricAnomaly(double mna, float ecc) {
    // use newton-raphson to approximate E.
    // f(x) = E - e*sin E - M
    // f'(x) = 1 - e*cos E
    float E = mna;

    for (int i = 0; i < 10; i++) {
        float f = E - ecc*sin(E)-mna;
        float fPrime = 1 - ecc*cos(E);

        E -= f/fPrime;
    }
    
    return E;
}

FPoint3 GetPointOnEllipse(float xLocal, float yLocal, double lan, double omega, float i) {
    // thanks superchil for this... monstrosity...
    float x = xLocal * (cos(omega) * cos(lan) - sin(omega) * sin(lan) * cos(i)) - yLocal * (sin(omega) * cos(lan) + cos(omega) * sin(lan) * cos(i));
    float y = xLocal * (cos(omega) * sin(lan) + sin(omega) * cos(lan) * cos(i)) - yLocal * (sin(omega) * sin(lan) - cos(omega) * cos(lan) * cos(i));
    float z = xLocal * (sin(omega) * sin(i))                                    + yLocal * (cos(omega) * sin(i));

    FPoint3 point = {x, y, z};
    return point;
}

Point RenderOrbit(OrbitParams orbit, Point center) {
    float ecc = orbit.eccentricity;
    double lan = orbit.lan;
    double omega = orbit.lpe - lan;
    float i = orbit.inclination;
    
    int a = orbit.smaxis;
    int b = (int)(sqrt(1-ecc*ecc)*a); // semi minor axis
    int max = (a > b) ? a : b;
    float step = 0.05/max;

    int focusShift = a * ecc;
    int xc = center.x;
    int yc = center.y;

    // render elipse
    for (float theta = 0.0; theta < M_TAU; theta += step) {
        float xLocal = a * cos(theta) - focusShift;
        float yLocal = b * sin(theta);
        
        FPoint3 point = GetPointOnEllipse(xLocal, yLocal, lan, omega, i);

        float cam_y = point.y * sin(VIEW_RAD) - point.z * cos(VIEW_RAD);

        mvaddch(yc-cam_y, xc+point.x, '.');
    }

    // render plane
    float E = CalculateEccentricAnomaly(orbit.mna, orbit.eccentricity);
    int planetXLocal = a * cos(E) - focusShift;
    int planetYLocal = b * sin(E);
    
    FPoint3 planetPos = GetPointOnEllipse(planetXLocal, planetYLocal, lan, omega, i);
    float cam_y = planetPos.y * sin(VIEW_RAD) - planetPos.z * cos(VIEW_RAD);
    mvaddch(yc-cam_y, xc+planetPos.x, 'O');
   
    Point planetRenderPos = {xc+planetPos.x, yc-cam_y};
    return planetRenderPos;
}

void RenderName(Point pos, char* name) {
    mvaddstr(pos.y-1, pos.x+2, name);
}

void RenderScene(Scene scene) {
    Point center = scene.center;
    
    // render planet orbits and the planet themselves
    Point planetPositions[scene.planetCount];
    for (int i = 0; i < scene.planetCount; i++) {
        planetPositions[i] = RenderOrbit(scene.planets[i].orbitparams, center);
    }

    // render planet names
    for (int i = 0; i < scene.planetCount; i++) {
        RenderName(planetPositions[i], scene.planets[i].name);
    }

    mvaddch(center.y, center.x, '*');
}
