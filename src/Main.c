#include <ncurses.h>
#include "In/input.h"
#include "In/cli.h"
#include "In/systemparser.h"
#include "Simulation/simulation.h"
#include "Simulation/render.h"
#include "Utils/util.h"

int main(int charc, char* argv[]) {
    ScanAvailableSystems();

    Initncurses();
    InitBinds();
    
    Scene mainScene = Scene_New();
    CLIConfig config = ParseCLI(charc, argv);
    InitScenePlanetsFromSystem(&mainScene, config.system);

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
