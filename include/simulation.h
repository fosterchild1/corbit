#ifndef SIMULATION_H
#define SIMULATION_H

#include "planet.h"

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Planet* planets;
    int planetCount;
    int planetCapacity;

    Point center;
    
    int elapsedTime;
} Scene;

void AddToScene(Scene* scene, Planet* planet);

void StepSimulation(Scene* scene, int seconds);

#endif
