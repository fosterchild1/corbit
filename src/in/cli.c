#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "cli.h"

const char* HELP_TEXT = "Usage: corbit [OPTIONS]...\n" 
"CLI arguments:\n"
"-t --time [TIME]       advance TIME step each tick\n"
"-s --system [SYSTEM]     system to simulate\n"
"Available systems: sol,jov,sat,ura,nep,plu,ker";

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
    exit(EXIT_SUCCESS);
}

void HandleArgument(CLIConfig* config, char* key, char* value) {
    // mega ugly
    if (value == NULL && (strcmp(key, "h")==0 || strcmp(key, "help")==0)) DisplayHelpText();
    if (value == NULL) return;

    if (strcmp(key, "t")==0 || strcmp(key, "time")==0) { config->time = StrToInt(value); return; }

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
            lastArg = fullArg + (isLongForm ? 2 : 1);
            continue;
        }
        
        HandleArgument(&config, lastArg, fullArg);
    }

    HandleArgument(&config, lastArg, NULL);
    
    return config;
}
