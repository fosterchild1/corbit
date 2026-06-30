#include <math.h>
#include "../include/scene.h"

#define M_PI 3.14159265358979323846

void StepSimulation(Scene* scene, int seconds) {
    for (int i = 0; i < scene->planetCount; i++) {
        Planet* planet = &scene->planets[i];
        OrbitParams* orbit = &planet->orbitparams;

        double delta = (2*M_PI)/sqrt(pow(orbit->smaxis, 3)) * seconds;
        orbit->mna += delta;
    } 
}
