#include <time.h>
#include <math.h>
#include <ncurses.h>
#include "In/systemparser.h"
#include "Simulation/simulation.h"
#include "Utils/util.h"

static const int TIME_BETWEEN_CHANGE = 60 * 3;

static long lastSystemChange = 0; 

void SwitchRandomSystem(Scene* scene) {
    int systemIdx = RandInt(0, AVAILABLE_SYSTEMS->len-1);
    Scene_SwitchSystem(scene, ((char**)AVAILABLE_SYSTEMS->data)[systemIdx]);

    lastSystemChange = time(NULL);
}

void StepSimulation(Scene* scene, int delta, bool isScreensaver) {
    scene->elapsedTime += delta;
    float timeSeconds = delta/100.0;

    // advance planet deltas
    for (int i = 0; i < scene->planetArray.len; i++) {
        Planet* planet = ((Planet**)scene->planetArray.data)[i];
        OrbitParams* orbit = &planet->orbitparams;

        // get delta and keep between 0-tau
        double deltaOrbit = M_TAU/sqrt(pow(orbit->smaxis, 3)) * timeSeconds;
        orbit->mna += deltaOrbit;
        if (orbit->mna > M_TAU) orbit->mna -= M_TAU;
    }

    // handle screensaver mode
    // TODO: change these hardcoded values
    if (!isScreensaver) return;
    // rotate camera slightly each step
    RotateScene(scene, 0, 0, 0.001 * delta);

    Camera sceneCam = scene->camera;
    sceneCam.viewAngle += 0.01 * delta;
    if (sceneCam.viewAngle > 360) sceneCam.viewAngle-=360;

    // system changing for screensaver
    if (lastSystemChange == 0) lastSystemChange = time(NULL);
    if (time(NULL) - lastSystemChange >= TIME_BETWEEN_CHANGE) SwitchRandomSystem(scene);
}

void RotateScene(Scene* scene, float lpe, float lan, float inc) {
    for (int i = 0; i < scene->planetArray.len; i++) {
        Planet* planet = ((Planet**)scene->planetArray.data)[i];
        OrbitParams* orbit = &planet->orbitparams;

        orbit->lpe += lpe;
        orbit->lan += lan;
        orbit->inclination += inc;

        if (orbit->lpe > M_TAU) orbit->lpe -= M_TAU;
        if (orbit->lan > M_TAU) orbit->lan -= M_TAU;
        if (orbit->inclination > M_TAU) orbit->inclination -= M_TAU;
    } 
}
