#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include "In/systemparser.h"
#include "Simulation/scene.h"
#include "Utils/util.h"

static short colorID=16;

void InitPlanetColors(short id, Color color) {
    // convert them into ncurses format
    short planetR = (color.R * 1000) / 255;
    short planetG = (color.G * 1000) / 255;
    short planetB = (color.B * 1000) / 255;

    // darken the orbit colors
    short orbitR = (DimColor(color.R) * 1000) / 255;
    short orbitG = (DimColor(color.G) * 1000) / 255;
    short orbitB = (DimColor(color.B) * 1000) / 255; 
    
    init_color(id, planetR, planetG, planetB);
    init_color(id+1, orbitR, orbitG, orbitB);

    init_pair(id, id, -1); // -1 = bg color
    init_pair(id+1, id+1, -1);
}

Planet CreatePlanet(OrbitParams* orbit, Color* color, char* name) {
    // initialize color
    if (colorID > COLORS) {
        // color limit reached, choose random one
        color->colorID = RandInt(17, COLORS);

    } else {
        InitPlanetColors(colorID, *color);
        color->colorID = colorID;
        colorID += 2;
    }

    return (Planet){*orbit, *color, name, .planetID = -1};
}

void UpdateViewAngle(Scene* scene, float rotAmt) {
    Camera* camera = &scene->camera;

    camera->viewAngle += rotAmt;
    if (camera->viewAngle > 360) camera->viewAngle-=360;
    if (camera->viewAngle < 0) camera->viewAngle+=360;

    float viewRad = rad(camera->viewAngle);
    camera->viewSin = sinf(viewRad); camera->viewCos = cosf(viewRad);
}

void AddToScene(Scene* scene, Planet* planet) {
    Array_Insert(&scene->planetArray, planet);
    planet->planetID = scene->planetArray.len;
}

void SwitchScene(Scene* scene, char* system) {
    CleanScene(scene);
    InitScene(scene, system);
}

void CleanScene(Scene* scene) {
    Planet* planets = (Planet*)scene->planetArray.data; (void)planets;
    for (int i = 0; i < scene->planetArray.len; i++) {
        free(planets[i].name);
    }

    Array_Free(&scene->planetArray);
}
