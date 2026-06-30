#include <ncurses.h>
#include "../include/planet.h"
#include "../include/simulation.h"
#include "../include/render.h"

int main() {
    int width, height;
	initscr(); noecho(); curs_set(0);
    getmaxyx(stdscr, height, width);
    
    Scene tes = {NULL, 0, 0, {width/2, height/2}, 0};

    OrbitParams t = {17, 0.6, 0, 0.6, 0.0, 0};
    Planet te = {t, {255, 255, 255}, "test"};
    AddToScene(&tes, &te);

    OrbitParams t2 = {30, 3.4, 1.3, 0.5, 0.8, 0.5};
    Planet te2 = {t2, {255, 255, 255}, "test2"};
    AddToScene(&tes, &te2);

    while (true) {
        RenderScene(tes);
        StepSimulation(&tes, 1);
        refresh();

	    getch();
        erase();
    }

	endwin();
	return 0;
}
