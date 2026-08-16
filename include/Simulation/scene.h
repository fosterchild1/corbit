#pragma once

#include <stdbool.h>
#include "planet.h"

typedef struct {
    float viewAngle;
    float zoom;
} Camera;

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
    Camera camera;
    
    int elapsedTime;
} Scene;

Planet CreatePlanet(OrbitParams* params, Color* color, char* name);

void AddToScene(Scene* scene, Planet* planet);
void RemoveFromScene(Scene* scene, Planet* planet);
void CleanScene(Scene* scene);

void StepSimulation(Scene* scene, int delta, bool isScreensaver);
void RotateScene(Scene* scene, float lpe, float lan, float inc);
