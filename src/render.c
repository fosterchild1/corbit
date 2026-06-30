#include <ncurses.h>
#include <math.h>
#include "../include/simulation.h"

#define M_PI 3.14159265358979323846

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

Point RenderOrbit(OrbitParams orbit, Point center) {
    float ecc = orbit.eccentricity;
    double w = orbit.lpe;
    
    int a = orbit.smaxis;
    int b = (int)(sqrt(1-ecc*ecc)*a); // semi minor axis
    int max = (a > b) ? a : b;
    float step = 0.005/max;

    int focusShift = a * ecc;
    int xc = center.x;
    int yc = center.y;

    // render elipse
    for (float theta = 0.0; theta < 2 * M_PI; theta += step) {
        float xLocal = a * cos(theta) - focusShift;
        float yLocal = b * sin(theta);
        
        int x = round(xc + (xLocal * cos(w) - yLocal * sin(w)));
        int y = round(yc - (xLocal * sin(w) + yLocal * cos(w)));
        mvaddch(y, x, '.');
    }

    // render planet
    float E = CalculateEccentricAnomaly(orbit.mna, orbit.eccentricity);
    int planetXLocal = a * cos(E) - focusShift;
    int planetYLocal = b * sin(E);

    int planetX = round(xc + (planetXLocal * cos(w) - planetYLocal * sin(w)));
    int planetY = round(yc - (planetXLocal * sin(w) + planetYLocal * cos(w)));
    mvaddch(planetY, planetX, 'O');
    
    Point planetPos = {planetX, planetY};
    return planetPos;
}

void RenderName(Point pos, char* name) {
    mvaddstr(pos.y-1, pos.x+2, name);
}

void RenderScene(Scene scene) {
    Point center = scene.center;
    mvaddch(center.y, center.x, '*');
    
    // render planet orbits and the planet themselves
    Point planetPositions[scene.planetCount];
    for (int i = 0; i < scene.planetCount; i++) {
        planetPositions[i] = RenderOrbit(scene.planets[i].orbitparams, center);
    }

    // render planet names
    for (int i = 0; i < scene.planetCount; i++) {
        RenderName(planetPositions[i], scene.planets[i].name);
    }
}
