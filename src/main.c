#include <ncurses.h>
#include "../include/planet.h"
#include "../include/scene.h"
#include "../include/render.h"

#define M_PI 3.14159265358979323846

int main() {
    int width, height;
	initscr(); noecho();
    getmaxyx(stdscr, height, width);
    float ecc = 0;
    while (true) {
        ecc += 0.05;
        OrbitParams t = {17, 0.6, 0, ecc, 0.85, 0};
        Planet te = {t, {255, 255, 255}, "test"};
        Scene tes = {&te, 1, 1, {width/2, height/2}, 0};
        RenderScene(tes);
        refresh();

	    getch();
        clear();
    }

	endwin();
	return 0;
}
