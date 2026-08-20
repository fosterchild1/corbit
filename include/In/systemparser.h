#pragma once

#include "Simulation/scene.h"

extern Array* AVAILABLE_SYSTEMS;

void ScanAvailableSystems(void);
void FreeAvailableSystems(void);

void InitScenePlanetsFromSystem(Scene* scene, const char* system);
