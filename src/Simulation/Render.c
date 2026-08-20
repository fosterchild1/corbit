#define _XOPEN_SOURCE_EXTENDED 1

#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdalign.h>
#include "Simulation/scene.h"
#include "Utils/util.h"

static int8_t* depthBuf;

const wchar_t PLANET_CHAR[2] = {9679, '\0'};

void RenderOrbit(Planet planet, Camera camera, Point center) {
    OrbitParams orbit = planet.orbitparams;
    float ecc = orbit.eccentricity;

    // build trig array
    double lan = orbit.lan;
    double omega = orbit.lpe - lan;
    float i = orbit.inclination;
    float trigArr[6] = {sinf(omega), cosf(omega), sinf(lan), cosf(lan), sinf(i), cosf(i)};

    int a = orbit.smaxis * camera.zoom;
    int b = (int)(sqrt(1 - ecc*ecc) * a); // semi minor axis
    mvprintw(10, 10, "%d, %d, %f", a, b, ecc);
    float step = 1.0f/max(a, b);

    int xc = center.x; int xSize = xc * 2;
    int yc = center.y; int ySize = yc * 2;

    // render elipse
    attron(COLOR_PAIR(planet.color.colorID+1));

    int lastY = 0; int lastX = 0;
    for (float theta = 0.0; theta < M_TAU; theta += step) {
        // get camera x and y
        float xLocal = a * (cosf(theta) - ecc);
        float yLocal = b * sinf(theta);
        
        FPoint3 point = GetPointOnElipse(xLocal, yLocal, trigArr);
        float camY = (point.y * camera.viewSin - point.z * camera.viewCos) * INV_TERM_FONT_RATIO;
        float targetY = yc-camY; float targetX = xc+point.x;

        // ensure the difference is enough for the orbit char to not overlap with the previous
        // and that roundX and roundY will be on screen
        if ((fabs(targetY - lastY) < 0.5f && fabs(targetX - lastX) < 0.5f) || 
            (targetY + 0.5f >= ySize || targetX + 0.5f >= xSize)             ||
            (targetY < 0 || targetX < 0)) continue;

        int roundY = roundf(targetY); int roundX = roundf(targetX);

        // get depth
        int8_t depth = point.y * camera.viewCos + point.z * camera.viewSin;
        int depthIdx = (roundY * xSize) + roundX;
        if (depthBuf[depthIdx] <= depth) continue;

        // put orbit char
        depthBuf[depthIdx] = depth;
        lastY = roundY; lastX = roundX;
        mvaddch(roundY, roundX, ':');
    }
    
    attroff(COLOR_PAIR(planet.color.colorID+1));
}

void RenderPlanet(const Planet planet, const Camera camera, const Point center) {
    OrbitParams orbit = planet.orbitparams;
    float ecc = orbit.eccentricity;

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
    float camY = (planetPos.y * camera.viewSin - planetPos.z * camera.viewCos) / TERM_FONT_RATIO;
    // render planet
    int planetY = roundf(yc-camY); int planetX = roundf(xc+planetPos.x);
    attron(COLOR_PAIR(planet.color.colorID));
    mvaddwstr(planetY, planetX, PLANET_CHAR); 
   
    // render name
    char* name = planet.name;
    mvaddstr(planetY-1, planetX+2, name);
    attroff(COLOR_PAIR(planet.color.colorID));
}

void RenderScene(const Scene scene) {
    Point center = scene.center;
    Planet** planets = (Planet**)scene.planetArray.data;

    // handle depth buffer
    int bufSize = center.x * center.y * 4 * sizeof(int8_t);
    if (depthBuf == NULL) depthBuf = Safemalloc(bufSize);
    memset(depthBuf, INT8_MAX, bufSize);

    // render planet orbits
    for (int i = 0; i < scene.planetArray.len; i++) {
        RenderOrbit(*planets[i], scene.camera, center);
    }

    // render planets and their names above the orbits
    for (int i = 0; i < scene.planetArray.len; i++) {
        RenderPlanet(*planets[i], scene.camera, center);
    }

    mvaddch(center.y, center.x, '*');
}

void FreeDepthBuf(void) {
    if (depthBuf == NULL) return;
    free(depthBuf);
}
