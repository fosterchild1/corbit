#include <ncurses.h>
#include "../include/planet.h"
#include "../include/scene.h"
#include "../include/render.h"

#define M_PI 3.14159265358979323846

int main() {
    int width, height;
	initscr(); noecho();
    getmaxyx(stdscr,width, height);

    OrbitParams t = {3, 0, 0, 0, 0, 0};
    Planet te = {t, {255, 255, 255}, "test"};
    Scene tes = {&te, 1, 1, {width/2, height/2}, 0};
    RenderScene(tes);

	refresh();
	getch();
	endwin();
	return 0;
}
