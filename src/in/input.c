#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "render.h" 
#include "simulation.h"
#include "util.h"

const float ROT_AMT = M_TAU/200;
const char* FULL_TEXT = "1/3 zoom arrow keys rotate q quit h hide";

static InputBind bindFunctions[512];
static bool ShouldRenderBinds = true;
static int xPos = 0;


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
    scene->camera.zoom *= 1.05;
}

void ZoomOut(Scene* scene) {
    scene->camera.zoom = max(scene->camera.zoom * 0.95, 0.05);
}

void ExitProgram(Scene* scene) { 
    (void)scene; 
    FreeDepthBuf();
    CleanScene(scene);
    
    reset_color_pairs();
    endwin(); 
    exit(EXIT_SUCCESS); 
}

void ToggleBinds(Scene* scene) {
    (void)scene;
    ShouldRenderBinds = !ShouldRenderBinds;
}

void InitBinds(void) {
    bindFunctions[KEY_UP] = RotateUp;
    bindFunctions[KEY_DOWN] = RotateDown;
    bindFunctions[KEY_RIGHT] = RotateRight;
    bindFunctions[KEY_LEFT] = RotateLeft;

    bindFunctions['1'] = ZoomIn;
    bindFunctions['3'] = ZoomOut;

    bindFunctions['q'] = ExitProgram;
    bindFunctions['Q'] = ExitProgram;

    bindFunctions['h'] = ToggleBinds;
    bindFunctions['H'] = ToggleBinds;
}

void HandleInput(Scene* scene, int ip) {
    if (ip == ERR) return;
    
    if (ip >= 0 && ip < 512 && bindFunctions[ip] != NULL) {
        bindFunctions[ip](scene);
    }
}

void PutStr(int bottom, char* str, int color) {
    attron(COLOR_PAIR(color));
    mvaddstr(bottom, xPos, str);
    attroff(COLOR_PAIR(color));

    xPos += strlen(str);
}

void RenderBinds(void) {
    if (!ShouldRenderBinds) return;

    int height, width;
    getmaxyx(stdscr, height, width);
    (void)width;
    int bottom = height-2;
    xPos = (width - strlen(FULL_TEXT))/2;

    // ugly shit and so hardcoded...
    attron(A_BOLD);
    PutStr(bottom, "1/3 ", COLOR_BLUE); PutStr(bottom, "zoom ", COLOR_WHITE);
    PutStr(bottom, "arrow keys ", COLOR_BLUE); PutStr(bottom, "rotate ", COLOR_WHITE);
    PutStr(bottom, "q ", COLOR_BLUE); PutStr(bottom, "quit ", COLOR_WHITE);
    PutStr(bottom, "h ", COLOR_BLUE); PutStr(bottom, "hide ", COLOR_WHITE);
    attroff(A_BOLD);
}
