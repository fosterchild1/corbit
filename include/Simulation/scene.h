#pragma once

#include <stdbool.h>
#include "planet.h"
#include "Utils/array.h"

typedef struct {
    float viewAngle;
    float zoom;

    float viewSin;
    float viewCos;
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

void UpdateViewAngle(Scene* scene, float rotAmt);

Scene Scene_New(void);
void Scene_AddPlanet(Scene* scene, Planet* planet);
void Scene_SwitchSystem(Scene* scene, char* name);
void Scene_Clean(Scene* scene);
