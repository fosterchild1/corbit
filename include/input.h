#pragma once

#include "simulation.h"

typedef void (*InputBind)(Scene* scene);

void InitBinds(void);
void ToggleBinds(Scene* scene);
void RenderBinds(void);

void HandleInput(Scene* scene, int ip);
