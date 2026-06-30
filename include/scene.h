#ifndef SCENE_H
#define SCENE_H

#include "planet.h"

typedef struct {
    Planet* planets;
    int elapsedTime;
} Scene;

void AddToScene(Scene scene, Planet planet);

#endif
