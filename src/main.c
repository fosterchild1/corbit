#include <ncurses.h>
#include "../include/planet.h"
#include "../include/simulation.h"
#include "../include/render.h"

#define M_PI 3.14159265358979323846

int main() {
    int width, height;
	initscr(); noecho();
    getmaxyx(stdscr, height, width);
    
    Scene tes = {NULL, 0, 0, {width/2, height/2}, 0};

    OrbitParams t = {17, 0.6, 0, 0.6, 0.0, 0};
    Planet te = {t, {255, 255, 255}, "test"};
    AddToScene(&tes, &te);

    OrbitParams t2 = {30,33.4, 0, 0.5, 0.8, 0};
    Planet te2 = {t2, {255, 255, 255}, "test2"};
    AddToScene(&tes, &te2);

    while (true) {
        RenderScene(tes);
        StepSimulation(&tes, 1);
        refresh();

	    getch();
        clear();
    }

	endwin();
	return 0;
}
