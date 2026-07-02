#include <ncurses.h>
#include <stdlib.h>
#include "../include/input.h"
#include "../include/simulation.h"
#include "../include/mathutil.h"

const float ROT_AMT = M_TAU/200;

static InputBind bindFunctions[512];

static bool ShouldRenderBinds = true;

void RotateUp(Scene* scene) {
    (void)scene;
    Camera* camera = &scene->camera;

    camera->viewAngle += ROT_AMT*180;
    if (camera->viewAngle > 360) camera->viewAngle-=360;
}

void RotateDown(Scene* scene) {
    (void)scene;
    Camera* camera = &scene->camera;

    camera->viewAngle -= ROT_AMT*180;
    if (camera->viewAngle > 360) camera->viewAngle-=360;
}

void RotateRight(Scene* scene) {
    RotateScene(scene, 0, 0, ROT_AMT);
}

void RotateLeft(Scene* scene) {
    RotateScene(scene, 0, 0, -ROT_AMT);
}

void ZoomIn(Scene* scene) {
    scene->camera.zoom += 0.05;
}

void ZoomOut(Scene* scene) {
    scene->camera.zoom = max(scene->camera.zoom - 0.05, 0.05);
}

void ExitProgram(Scene* scene) { (void)scene; endwin(); exit(0); }

void ToggleBinds() {
    ShouldRenderBinds = !ShouldRenderBinds;
}

void InitBinds() {
    bindFunctions[KEY_UP] = RotateUp;
    bindFunctions[KEY_DOWN] = RotateDown;
    bindFunctions[KEY_RIGHT] = RotateRight;
    bindFunctions[KEY_LEFT] = RotateLeft;

    bindFunctions['1'] = ZoomIn;
    bindFunctions['3'] = ZoomOut;

    bindFunctions['q'] = ExitProgram;
    bindFunctions['Q'] = ExitProgram;

    bindFunctions[KEY_F(1)] = ToggleBinds;
}

void HandleInput(Scene* scene, int ip) {
    if (ip == ERR) return;
    mvprintw(10, 10, "%d", ip); 
    if (ip >= 0 && ip < 512 && bindFunctions[ip] != NULL) {
        bindFunctions[ip](scene);
    }
}

void ReverseStr(int y, int x, char* str) {
    attron(A_REVERSE);
    mvaddstr(y, x, str);
    attroff(A_REVERSE);
}

void RenderBinds() {
    if (!ShouldRenderBinds) return;

    int height, width;
    getmaxyx(stdscr, height, width);
    (void)width;
    int bottom = height-1;
    
    // ugly shit...
    ReverseStr(bottom, 0, "1"); mvaddstr(bottom, 1, "zoom in ");
    ReverseStr(bottom, 9, "3"); mvaddstr(bottom, 10, "zoom out ");
    ReverseStr(bottom, 19, "arrow keys"); mvaddstr(bottom, 29, "rotate scene ");
    ReverseStr(bottom, 42, "Q"); mvaddstr(bottom, 43, "quit ");
    ReverseStr(bottom, 48, "F1"); mvaddstr(bottom, 50, "hide binds");
}
