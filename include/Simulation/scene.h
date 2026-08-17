#pragma once

#include <stdbool.h>
#include "planet.h"
#include "Utils/array.h"

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
    Array planetArray;

    Point center;
    Camera camera;
    
    int elapsedTime;
} Scene;

Planet CreatePlanet(OrbitParams* params, Color* color, char* name);

void AddToScene(Scene* scene, Planet* planet);
void RemoveFromScene(Scene* scene, Planet* planet);
void SwitchScene(Scene* scene, char* name);
void CleanScene(Scene* scene);
