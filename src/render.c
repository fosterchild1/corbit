#include <ncurses.h>
#include <math.h>
#include "../include/scene.h"

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
    char str[10];
    sprintf(str, "%f", mna);
    int a, b;
    getmaxyx(stdscr, a, b);
    mvprintw(b/2, a/2, str);
    return E;
}

void RenderOrbit(OrbitParams orbit, Point center) {
    float ecc = orbit.eccentricity;
    double w = orbit.lpe;
    
    int a = orbit.smaxis;
    int b = (int)(sqrt(1-ecc*ecc)*a); // semi minor axis
    int max = (a > b) ? a : b;
    float step = 0.5/max;

    int focusShift = a * ecc;
    int xc = center.x;
    int yc = center.y;

    // render elipse
    for (float theta = 0.0; theta < 2 * M_PI; theta += step) 
    {
        float xLocal = a * cos(theta) - focusShift;
        float yLocal = b * sin(theta);

        int x = (int)(xc + (xLocal * cos(w) - yLocal * sin(w)) + 0.5);
        int y = (int)(yc - (xLocal * sin(w) + yLocal * cos(w)) + 0.5);
        mvaddch(y, x, '.');
    }

    // render planet
    float E = CalculateEccentricAnomaly(orbit.mna, orbit.eccentricity);
    int planetXLocal = a * cos(E) - focusShift;
    int planetYLocal = b * sin(E);

    int planetX = (int)(xc + (planetXLocal * cos(w) - planetYLocal * sin(w)) + 0.5);
    int planetY = (int)(yc - (planetXLocal * sin(w) + planetYLocal * cos(w)) + 0.5);
    mvaddch(planetY, planetX, 'O');
}

void RenderScene(Scene scene) {
    Point center = scene.center;
    mvaddch(center.y, center.x, '*');
    RenderOrbit(scene.planets->orbitparams, center);
}
