#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include "In/systemparser.h"
#include "Simulation/scene.h"
#include "Utils/util.h"

static const int8_t DEFAULT_COLOR_COUNT = 16;

static int8_t colorID = DEFAULT_COLOR_COUNT;

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
        color->colorID = RandInt(DEFAULT_COLOR_COUNT + 1, COLORS);
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

Scene Scene_New(void) {
    int width, height;
    getmaxyx(stdscr, height, width);

    return (Scene){.planetArray = Array_New(sizeof(Planet*)), .center = {width/2, height/2}, .camera = {90, 1, 1, 0},
                       .elapsedTime = 0};
}

void Scene_AddPlanet(Scene* scene, Planet* planet) {
    Array_Insert(&scene->planetArray, &planet);
    planet->planetID = scene->planetArray.len;
}

void Scene_SwitchSystem(Scene* scene, const char* system) {
    Scene_Clean(scene);

    reset_color_pairs(); colorID = DEFAULT_COLOR_COUNT;
    
    InitScenePlanetsFromSystem(scene, system);
}

void Scene_Clean(Scene* scene) {
    Planet** planets = (Planet**)scene->planetArray.data;
    for (int i = 0; i < scene->planetArray.len; i++) {
        free(planets[i]->name);
        free(planets[i]);
    }

    Array_Free(&scene->planetArray);
}
