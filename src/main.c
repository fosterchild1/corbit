#include <ncurses.h>
#include "input.h"
#include "simulation.h"
#include "render.h"
#include "cli.h"
#include "systemparser.h"

void Initncurses(void) {
    initscr();
    noecho();
    cbreak(); 
    curs_set(0);
    start_color();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
}

int main(int charc, char* argv[]) {
    CLIConfig config = ParseCLI(charc, argv);

    int width, height;
    Initncurses();
    getmaxyx(stdscr, height, width);
    
    Scene mainScene = {NULL, 0, 0, {width/2, height/2}, {90, 1}, 0};
    InitScene(&mainScene, config.system);

    InitBinds();

    while (true) {
        RenderScene(mainScene);
        RenderBinds();
        StepSimulation(&mainScene, config.time);
        HandleInput(&mainScene, getch());
        refresh();

        napms(16);
        erase();
    }

	endwin();
	return 0;
}
