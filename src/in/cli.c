#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "cli.h"

const char* HELP_TEXT = "Usage: corbit [OPTIONS]... \
CLI arguments: \
-t --time [TIME]       advance TIME step each tick \
-s --system [SYSTEM]     system to simulate \
\
Available systems: sol,jov,sat,nep,ura,ker,plu";

const CLIConfig DEFAULT_CONFIG = {1, "sol"};

int ContainsEqual(char* str) {
    char* p = str;
    while (*p != '\0') {
        if (*p == '=') return str-p;
        p++;
    }

    return -1;
}

void DisplayHelpText(void) {
    printf("%s", HELP_TEXT);

    endwin();
    exit(EXIT_SUCCESS);
}

CLIConfig ParseCLI(int argc, char* argv[]) {
    if (argc == 0) return DEFAULT_CONFIG;
    CLIConfig config = DEFAULT_CONFIG;
   
    char* lastArg;
    for (int i = 1; i < argc; i++) {
        char* fullArg = argv[i];
        
        bool isKey = (fullArg[0] == '-');
        bool isLongForm = (isKey && fullArg[1] == '-'); // e.g. "--time" or "--system"
        
        // TODO: Support for "any=x" style arguments
        if (isKey) {
            lastArg = fullArg + (isLongForm ? 2 : 1);
            continue;
        }
        
        if (lastArg == NULL) continue;

        // handle arguments.. ugly.
        if (strcmp(lastArg, "t") || strcmp(lastArg, "time")) { config.time = strToInt(fullArg); continue; }

        if (strcmp(lastArg, "s") || strcmp(lastArg, "system")) { config.system = fullArg; continue; }
    }
    
    return config;
}
