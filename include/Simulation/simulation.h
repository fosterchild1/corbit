#pragma once

#include <stdbool.h>
#include "scene.h"

void StepSimulation(Scene* scene, int delta, bool isScreensaver);
void RotateScene(Scene* scene, float lpe, float lan, float inc);
