#pragma once

#include "simulation.h"

extern const char* AVAILABLE_SYSTEMS[7];

void InitScene(Scene* scene, char* system);
void SwitchScene(Scene* scene, char* system);
