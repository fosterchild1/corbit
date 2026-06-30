#include <math.h>
#include <stdlib.h>
#include "../include/simulation.h"

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
        orbit->inclination+=0.01;
        if (orbit->mna > M_TAU) orbit->mna -= M_TAU;
    } 
}
