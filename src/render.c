#include <ncurses.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "simulation.h"
#include "util.h"

static int8_t* depthBuf;

void RenderOrbit(Planet planet, Camera camera, Point center) {
    OrbitParams orbit = planet.orbitparams;
    float ecc = orbit.eccentricity;
    float viewRad = rad(camera.viewAngle);

    // build trig array
    double lan = orbit.lan;
    double omega = orbit.lpe - lan;
    float i = orbit.inclination;
    float trigArr[6] = {sinf(omega), cosf(omega), sinf(lan), cosf(lan), sinf(i), cosf(i)};

    int a = orbit.smaxis * camera.zoom;
    int b = (int)(sqrt(1 - ecc*ecc) * a); // semi minor axis

    float step = 1.0/max(a, b);

    int xc = center.x;
    int yc = center.y;

    // render elipse
    int lastY = 0; int lastX = 0;
    for (float theta = 0.0; theta < M_TAU; theta += step) {
        // get camera x and y
        float xLocal = a * (cosf(theta) - ecc);
        float yLocal = b * sinf(theta);
        
        FPoint3 point = GetPointOnElipse(xLocal, yLocal, trigArr);

        float camY = (point.y * sinf(viewRad) - point.z * cosf(viewRad)) / TERM_FONT_RATIO;

        int targetY = (yc-camY); int targetX = xc+point.x;
        // ensure targetY and targetX are on screen
        if ((targetY == lastY && targetX == lastX) || 
            (targetY >= yc*2 || targetX >= xc*2)   ||
            (targetY < 0 || targetX < 0)) continue;

        // get depth
        int8_t depth = point.y * cosf(viewRad) + point.z * sinf(viewRad);
        int depthIdx = (targetY * xc * 2) + targetX;
        if (depthBuf[depthIdx] <= depth) continue;
        
        depthBuf[depthIdx] = depth;
        lastY = targetY; lastX = targetX;
        mvaddch(targetY, targetX, ':' | COLOR_PAIR(planet.color.colorID+1));
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
    
    int a = orbit.smaxis * camera.zoom;
    int b = (int)(sqrt(1 - ecc*ecc) * a); // semi minor axis

    int xc = center.x;
    int yc = center.y;

    // render planet via keplers equation
    float E = CalculateEccentricAnomaly(orbit.mna, ecc);
    float planetXLocal = a * (cosf(E) - ecc);
    float planetYLocal = b * sinf(E);
    
    FPoint3 planetPos = GetPointOnElipse(planetXLocal, planetYLocal, trigArr);
    float camY = (planetPos.y * sin(viewRad) - planetPos.z * cos(viewRad)) / TERM_FONT_RATIO;

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

    // handle depth buffer
    int bufSize = center.x * center.y * 4;
    if (depthBuf == NULL) {
        depthBuf = malloc(bufSize * sizeof(int8_t));
        if (depthBuf == NULL) exit(0);
    }
    memset(depthBuf, INT8_MAX, bufSize);

    // render planet orbits
    for (int i = 0; i < scene.planetCount; i++) {
        RenderOrbit(scene.planets[i], scene.camera, center);
    }

    // render planets and their names above the orbits
    for (int i = 0; i < scene.planetCount; i++) {
        RenderPlanet(scene.planets[i], scene.camera, center);
    }

    mvaddch(center.y, center.x, '*');
}
