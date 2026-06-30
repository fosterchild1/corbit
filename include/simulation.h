#ifndef SIMULATION_H
#define SIMULATION_H

#include "planet.h"

#define M_PI 3.14159265358979323846
#define M_TAU 2*M_PI

#define VIEW_ANGLE 0
#define VIEW_RAD (VIEW_ANGLE * M_PI/180.0)

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    float x;
    float y;
    float z;
} FPoint3;

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
