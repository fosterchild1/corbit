#ifndef CLI_H
#define CLI_H

typedef struct {
    int time;
    char* system;
} CLIConfig;

CLIConfig ParseCLI(int charc, char* argv[]);

#endif
