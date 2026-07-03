#ifndef INPUT_H
#define INPUT_H

#include "simulation.h"

typedef void (*InputBind)(Scene* scene);

void InitBinds(void);
void RenderBinds(void);

void HandleInput(Scene* scene, int ip);

#endif
