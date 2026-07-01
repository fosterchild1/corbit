#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../include/simulation.h"

short colorID=16;
Planet CreatePlanet(OrbitParams* orbit, Color* color, char* name) {
    // initialize color
    if (colorID > COLORS) {
        // color limit reached, choose random one
        int randId = (rand() % (COLORS - 31)) + 16;
        color->colorID = randId;

    } else {
        // convert them into ncurses format
        short r = (color->R * 1000) / 255;
        short g = (color->G * 1000) / 255;
        short b = (color->B * 1000) / 255;

        init_color(colorID, r, g, b);
        init_pair(colorID, colorID, COLOR_BLACK);

        color->colorID = colorID;
        colorID++;
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
        if (temp == NULL) exit(0);
        
        scene->planetCapacity = newSize;
        scene->planets = temp;
    }
    
    scene->planets[neededSize - 1] = *planet;
    scene->planetCount += 1;
}

void StepSimulation(Scene* scene, int seconds) {
    for (int i = 0; i < scene->planetCount; i++) {
        Planet* planet = &scene->planets[i];
        OrbitParams* orbit = &planet->orbitparams;

        // get delta and keep between 0-tau
        double delta = M_TAU/sqrt(pow(orbit->smaxis, 3)) * seconds;
        orbit->mna += delta;
        if (orbit->mna > M_TAU) orbit->mna -= M_TAU;
    } 
}
