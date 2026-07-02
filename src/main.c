#include <ncurses.h>
#include "../include/planet.h"
#include "../include/input.h"
#include "../include/simulation.h"
#include "../include/render.h"

void Initncurses() {
    initscr();
    noecho(); 
    cbreak(); 
    curs_set(0);
    start_color();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
}

int main() {
    int width, height;
    Initncurses();
    getmaxyx(stdscr, height, width);

    InitBinds();
    Scene mainScene = {NULL, 0, 0, {width/2, height/2}, {90, 1}, 0};
    
    OrbitParams t = {17, 3.4, 1.3, 0.6, 0.8, -0.3};
    Color tcolor = {255, 255, 0, 0};
    Planet te = CreatePlanet(&t, &tcolor, "test");
    AddToScene(&mainScene, &te);

    OrbitParams t2 = {30, 3.4, 1.3, 0.5, 0.8, 0.5};
    Color tcolor2 = {255, 0, 0, 0};
    Planet te2 = CreatePlanet(&t2, &tcolor2, "test2");
    AddToScene(&mainScene, &te2);
    
    while (true) {
        RenderScene(mainScene);
        RenderBinds();
        StepSimulation(&mainScene, 1);
        HandleInput(&mainScene, getch());

        napms(16);
        refresh();
        erase();
    }

	endwin();
	return 0;
}
