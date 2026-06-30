#include <ncurses.h>
#include <math.h>
#include "../include/scene.h"

#define M_PI 3.14159265358979323846

void RenderOrbit(OrbitParams orbit, Point center) {

    int ecc = orbit.eccentricity;

    int xc = center.x;
    int yc = center.y;

    int a = orbit.smaxis;
    int b = sqrt(1-ecc*ecc)*a; // semi minor axis
    int max = (a > b) ? a : b;
    float step = 1.0/max;

    for (float theta = 0.0; theta < 2 * M_PI; theta += step) 
    {
        int x = (int)(xc + a*cos(theta) + 0.5);
        int y = (int)(yc + b*sin(theta) + 0.5);
        mvprintw(x, y, "+");
    }
}


void RenderScene(Scene scene) {
    Point center = scene.center;
    mvaddch(center.x, center.y, '*');
    RenderOrbit(scene.planets->orbitparams, center);
}
