#pragma once

#include <stdbool.h>

typedef struct {
    int time;
    char* system;
    bool screensaver;
} CLIConfig;

CLIConfig ParseCLI(int charc, char* argv[]);
