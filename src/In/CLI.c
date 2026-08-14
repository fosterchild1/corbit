#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "input.h"
#include "systemparser.h"

const char* HELP_TEXT = "Usage: corbit [OPTION]...\n" 
"Options:\n"
"-t --time [TIME]       advance TIME step each tick\n"
"-s --system [SYSTEM]   system to simulate\n"
"-c --screensaver       screensaver mode. speed is affected by -t\n"  
"Available systems: ";

const CLIConfig DEFAULT_CONFIG = {.time=1, .system="sol", .screensaver=false};

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

    int arrSize = sizeof(AVAILABLE_SYSTEMS) / sizeof(AVAILABLE_SYSTEMS[0]);
    for (int i = 0; i < arrSize; i++) {
        if (i == arrSize - 1)
            puts(AVAILABLE_SYSTEMS[i]);
        else 
            printf("%s,", AVAILABLE_SYSTEMS[i]);
    }

    exit(EXIT_SUCCESS);
}

void HandleArgument(CLIConfig* config, char* key, char* value) {
    // mega ugly
    // non-value arguments
    if (strcmp(key, "h")==0 || strcmp(key, "help")==0) DisplayHelpText();

    if (strcmp(key, "c")==0 || strcmp(key, "screensaver")==0) {config->screensaver = true; return; }

    // value arguments
    if (value == NULL) return;

    if (strcmp(key, "t")==0 || strcmp(key, "time")==0) { 
        double val = strtod(value, NULL);
        config->time = val;
        return; 
    }

    if (strcmp(key, "s")==0 || strcmp(key, "system")==0) { config->system = value; return; }
}

CLIConfig ParseCLI(int argc, char* argv[]) {
    if (argc <= 1) return DEFAULT_CONFIG;
    CLIConfig config = DEFAULT_CONFIG;

    char* lastArg = NULL;
    for (int i = 1; i < argc; i++) {
        char* fullArg = argv[i];
        
        bool isKey = (fullArg[0] == '-');
        bool isLongForm = (isKey && fullArg[1] == '-'); // e.g. "--time" or "--system"
        
        // TODO: Support for "any=x" style arguments
        if (isKey) {
            if (lastArg != NULL) { // two keys in a row, for example: -c -s jov, -c doesnt require any value since its a bool
                HandleArgument(&config, lastArg, NULL);
            }

            lastArg = fullArg + (isLongForm ? 2 : 1);
            continue;
        }

        HandleArgument(&config, lastArg, fullArg);
        lastArg = NULL;
    }

    // last argument may not be finished, set its value to NULL
    if (lastArg != NULL) { 
        HandleArgument(&config, lastArg, NULL);
    }

    if (config.screensaver) ToggleBinds((Scene*)1); // hide keybinds, cast to Scene* as it isnt used anyway
    
    return config;
}
