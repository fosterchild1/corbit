#include <math.h>
#include <stdlib.h>
#include "../include/simulation.h"

#define M_PI 3.14159265358979323846

void AddToScene(Scene* scene, Planet* planet) {
    int currSize = scene->planetCapacity;
    int neededSize = scene->planetCount + 1;

    if (neededSize > currSize) {
        if (currSize == 0) currSize = 1;

        Planet* temp = realloc(scene->planets, sizeof(Planet)*currSize*2);
        if (temp == NULL) exit(0);
        
        scene->planetCapacity = currSize * 2;
        scene->planets = temp;
    }
    
    scene->planets[neededSize - 1] = *planet;
    scene->planetCount += 1;
}

void StepSimulation(Scene* scene, int seconds) {
    for (int i = 0; i < scene->planetCount; i++) {
        Planet* planet = &scene->planets[i];
        OrbitParams* orbit = &planet->orbitparams;

        double delta = (2*M_PI)/sqrt(pow(orbit->smaxis, 3)) * seconds;
        orbit->mna += delta;
    } 
}
