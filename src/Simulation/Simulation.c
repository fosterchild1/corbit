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

double ClampTAU(double angle) {
    double w = fmod(angle, M_TAU);
    return (w < 0.0 ? w + M_TAU : w);
}

void AdvancePlanets(Scene* scene, float delta) {
    for (int i = 0; i < scene->planetArray.len; i++) {
        Planet* planet = ((Planet**)scene->planetArray.data)[i];
        OrbitParams* orbit = &planet->orbitparams;

        // get delta and keep between 0-tau
        double deltaOrbit = M_TAU/sqrt(pow(orbit->smaxis, 3)) * delta;
        orbit->mna = ClampTAU(orbit->mna + deltaOrbit);
    }
}

void AdvanceScreensaver(Scene* scene, float delta) {
    RotateScene(scene, 0, 0, delta/10.0);

    Camera sceneCam = scene->camera;
    sceneCam.viewAngle = fmod(sceneCam.viewAngle + delta, 360);

    // system changing for screensaver
    if (lastSystemChange == 0) lastSystemChange = time(NULL);
    if (time(NULL) - lastSystemChange >= TIME_BETWEEN_CHANGE) SwitchRandomSystem(scene);
}

void StepSimulation(Scene* scene, int delta, bool isScreensaver) {
    scene->elapsedTime += delta;
    float deltaSeconds = delta/100.0;
    AdvancePlanets(scene, deltaSeconds);
    
    if (!isScreensaver) return;
    AdvanceScreensaver(scene, deltaSeconds);
}

void RotateScene(Scene* scene, float lpe, float lan, float inc) {
    for (int i = 0; i < scene->planetArray.len; i++) {
        Planet* planet = ((Planet**)scene->planetArray.data)[i];
        OrbitParams* orbit = &planet->orbitparams;

        orbit->lpe = ClampTAU(orbit->lpe + lpe);
        orbit->lan = ClampTAU(orbit->lan + lan);
        orbit->inclination = ClampTAU(orbit->inclination + inc);
    } 
}
