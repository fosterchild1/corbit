#include <math.h>
#include <ncurses.h>
#include "util.h"

void StepSimulation(Scene* scene, int delta, bool isScreensaver) {
    scene->elapsedTime += delta;
    float time = delta/100.0;

    for (int i = 0; i < scene->planetCount; i++) {
        Planet* planet = &scene->planets[i];
        OrbitParams* orbit = &planet->orbitparams;

        // get delta and keep between 0-tau
        double deltaOrbit = M_TAU/sqrt(pow(orbit->smaxis, 3)) * time;
        orbit->mna += deltaOrbit;
        if (orbit->mna > M_TAU) orbit->mna -= M_TAU;
    }

    // TODO: change these hardcoded values
    if (!isScreensaver) return;
    RotateScene(scene, 0, 0, 0.001 * delta);

    Camera sceneCam = scene->camera;
    sceneCam.viewAngle += 0.01 * delta;
    if (sceneCam.viewAngle > 360) sceneCam.viewAngle-=360;
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
