#include <ncurses.h>
#include <stdlib.h>
#include "../include/input.h"
#include "../include/simulation.h"

const float ROT_AMT = M_TAU/100;

InputBind bindFunctions[512];

void RotateUp(Scene* scene) {
    (void)scene;

    VIEW_ANGLE += ROT_AMT*180;
    if (VIEW_ANGLE > 360) VIEW_ANGLE-=360;
}

void RotateDown(Scene* scene) {
    (void)scene;

    VIEW_ANGLE -= ROT_AMT*180;
    if (VIEW_ANGLE < 0) VIEW_ANGLE+=360;
}

void RotateRight(Scene* scene) {
    RotateScene(scene, 0, 0, ROT_AMT);
}

void RotateLeft(Scene* scene) {
    RotateScene(scene, 0, 0, -ROT_AMT);
}

/*void ZoomIn(Scene* scene) {
    //ZoomScene(scene, 0.5);
}

void ZoomOut(Scene* scene) {
    //ZoomScene(scene, -0.5);
}*/

void ExitProgram(Scene* scene) { (void)scene; endwin(); exit(0); }

void InitBinds() {
    bindFunctions[KEY_UP] = RotateUp;
    bindFunctions[KEY_DOWN] = RotateDown;
    bindFunctions[KEY_RIGHT] = RotateRight;
    bindFunctions[KEY_LEFT] = RotateLeft;

    //bindFunctions['1'] = ZoomIn;
    //bindFunctions['3'] = ZoomOut;

    bindFunctions['q'] = ExitProgram;
    bindFunctions['Q'] = ExitProgram;
}

void HandleInput(Scene* scene, int ip) {
    if (ip == ERR) return;
    mvprintw(10, 10, "%d", ip); 
    if (ip >= 0 && ip < 512 && bindFunctions[ip] != NULL) {
        bindFunctions[ip](scene);
    }
}
