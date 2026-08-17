#include <ncurses.h>
#include <locale.h>
#include "In/input.h"
#include "In/cli.h"
#include "In/systemparser.h"
#include "Simulation/simulation.h"
#include "Simulation/render.h"
#include "Utils/util.h"

void Initncurses(void) {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak(); 
    curs_set(0);

    if (has_colors()) {
        start_color();
        use_default_colors();
        InitDefaultColorPairs();
    }

    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
}

int main(int charc, char* argv[]) {
    CLIConfig config = ParseCLI(charc, argv);
    Initncurses();
    InitBinds();
    
    // init scene
    int width, height;
    getmaxyx(stdscr, height, width);

    Scene mainScene = {.planetArray = Array_New(sizeof(Planet)), .center = {width/2, height/2}, .camera = {90, 1, 1, 0},
                       .elapsedTime = 0};

    InitScene(&mainScene, config.system);

    while (true) {
        RenderScene(mainScene);
        RenderBinds();
        StepSimulation(&mainScene, config.time, config.screensaver);
        HandleInput(&mainScene, getch());
        refresh();

        napms(16);
        erase();
    }
    
	return 0;
}
