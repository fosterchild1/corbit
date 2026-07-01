#include <ncurses.h>
#include "../include/simulation.h"

typedef void (*InputBind)(Scene* scene);

InputBind bindFunctions[512];
const float ROT_AMT = M_TAU/100;

void RotateUp(Scene* scene) {
    RotateScene(scene, 0, 0, ROT_AMT);
}

void RotateDown(Scene* scene) {
    RotateScene(scene, 0, 0, -ROT_AMT);
}

void InitBinds() {
    bindFunctions[KEY_UP] = RotateUp;
    bindFunctions[KEY_DOWN] = RotateDown;
}

void HandleInput(Scene* scene, int ip) {
    if (ip == ERR) return;
    
    if (ip >= 0 && ip < 512 && bindFunctions[ip] != NULL) {
        bindFunctions[ip](scene);
    }
}
