#pragma once

#include "Simulation/scene.h"

#define SYSTEM_NAME_SIZE 4;

extern Array* AVAILABLE_SYSTEMS;

void ScanAvailableSystems(void);
void FreeAvailableSystems(void);

void InitScenePlanetsFromSystem(Scene* scene, const char* system);
