#include <stdlib.h>
#include <ncurses.h>
#include "Simulation/scene.h"
#include "util.h"

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
        int randId = (rand() % (COLORS - 31)) + 16;
        color->colorID = randId;

    } else {
        InitPlanetColors(colorID, *color);
        color->colorID = colorID;
        colorID += 2;
    }

    return (Planet){*orbit, *color, name, -1};
}

void AddToScene(Scene* scene, Planet* planet) {
    int currSize = scene->planetCapacity;
    int neededSize = scene->planetCount + 1;
    
    // resize if needed
    if (neededSize > currSize) {
        if (currSize == 0) currSize = 1;
        int newSize = currSize * 2;

        Planet* temp = realloc(scene->planets, sizeof(Planet) * newSize);
        if (temp == NULL) exit(EXIT_FAILURE);
        
        scene->planetCapacity = newSize;
        scene->planets = temp;
    }

    planet->planetID = scene->planetCount;

    scene->planetCount += 1;
    scene->planets[neededSize - 1] = *planet;
}

void RemoveFromScene(Scene* scene, Planet* planet) {
    int id = planet->planetID;
    if (id < 0) return; // not in scene
    free(scene->planets[id].name);
    planet->planetID = -1;

    // shift all planet indexes after the removing planet down by one
    int newSize = --scene->planetCount;

    for (int i = id; i < newSize; i++) {
        scene->planets[i] = scene->planets[i + 1];
        scene->planets[i].planetID = i;
    }

    if (newSize == 0) {
        free(scene->planets);
        scene->planets = NULL;
        scene->planetCapacity = 0;
        scene->planetCount = 0;

        return;
    }
    
    Planet* newScenePlanets = realloc(scene->planets, sizeof(Planet) * newSize);
    if (newScenePlanets == NULL) exit(EXIT_FAILURE);
    
    scene->planets = newScenePlanets;
    scene->planetCapacity = newSize;
}

void CleanScene(Scene* scene) {
    for (int i = 0; i < scene->planetCount; i++) {
        free(scene->planets[i].name);
    }
    free(scene->planets);
    scene->planets = NULL;

    scene->planetCount = 0;
    scene->planetCapacity = 0;
}
