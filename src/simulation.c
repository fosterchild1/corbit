#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "simulation.h"

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

    return (Planet){*orbit, *color, name};
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
    
    scene->planets[neededSize - 1] = *planet;
    scene->planetCount += 1;
}

void StepSimulation(Scene* scene, int seconds) {
    float time = seconds/100.0;

    for (int i = 0; i < scene->planetCount; i++) {
        Planet* planet = &scene->planets[i];
        OrbitParams* orbit = &planet->orbitparams;

        // get delta and keep between 0-tau
        double delta = M_TAU/sqrt(pow(orbit->smaxis, 3)) * time;
        orbit->mna += delta;
        if (orbit->mna > M_TAU) orbit->mna -= M_TAU;
    } 
}

void RotateScene(Scene* scene, float lpe, float lan, float inc) {
    for (int i = 0; i < scene->planetCount; i++) {
        Planet* planet = &scene->planets[i];
        OrbitParams* orbit = &planet->orbitparams;

        orbit->lpe += lpe;
        orbit->lan += lan;
        orbit->inclination += inc;

        if (orbit->lpe > M_TAU) orbit->lpe -= M_TAU;
        if (orbit->lan > M_TAU) orbit->lan -= M_TAU;
        if (orbit->inclination > M_TAU) orbit->inclination -= M_TAU;
    } 
}
