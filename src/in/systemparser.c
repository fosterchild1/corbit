#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "simulation.h"
#include "util.h"

/* example system:
.sol
-Mercury
#mna 0.91
#lan 4.15
..etc
*/

const char* systemsDir = "/.config/corbit/systems";

char* ReadFile(char* dir) {
    FILE* file = fopen(dir, "r");
    if (file == NULL) return "";

    // read file contents
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *string = malloc(fileSize + 1);
    if (string == NULL) exit(EXIT_FAILURE);

    fread(string, fileSize, 1, file);
    fclose(file);

    return string;
}

char* ReadSystemsFile(void) {
    // get file directory
    char* homeDir = getenv("HOME");
    int dirLen = strlen(homeDir) + strlen(systemsDir);
    char fileDir[dirLen];
    strcpy(fileDir, homeDir);
    strncat(fileDir, systemsDir, dirLen);
    
    return ReadFile(fileDir);
}

int FindNextChar(char* str, char ch, int startIdx) {
    if (startIdx + 1 > (int)strlen(str)) return -1;
    char* p = str + startIdx;

    while (*p != '\0') {
        if (*p == ch) return p - str;
        p++;
    }

    return -1;
}

char* ReadName(char* str, int startIdx) {
    if (startIdx + 1 > (int)strlen(str)) return "";
    char* p = str + startIdx;
    
    // loop till next space/newline
    while (*p != '\0') {
        if (*p != ' ' && *p != '\n') { p++; continue; }
        
        int nameLen = p - (str + startIdx);
        char* name = Strsub(str, startIdx, startIdx + nameLen);
        return name;
    }

    return "";
}

char* GetSystemConfig(char* contents, char* system) {
    int currIdx = 0;

    // find system, denoted by .systemname
    while (true) {
        int nameStartIdx = FindNextChar(contents, '.', currIdx);
        if (nameStartIdx == -1) return "";
        currIdx  = nameStartIdx + 1;

        char* systemName = ReadName(contents, currIdx);
        currIdx += strlen(systemName);

        if (strcmp(systemName, system) != 0) continue;
        break;
    }
    
    // find where the system config ends, either from the next system or by end of file
    int nextSystemIdx = FindNextChar(contents, '.', currIdx);
    nextSystemIdx = (nextSystemIdx == -1) ? (int)strlen(contents) - 1 : nextSystemIdx;

    return Strsub(contents, currIdx, nextSystemIdx);
}

Planet MakePlanetFromConfig(char* name, char* config, int startIdx, int endIdx) {
    OrbitParams orbit = {0, 0, 0, 0, 0, 0};
    Color color = {255, 255, 255, 0};

    int currIdx = startIdx;
    while (true) {
        // all variable names start with #, followed by their value
        int keyIdx = FindNextChar(config, '#', currIdx);
        if (keyIdx == -1 || keyIdx > endIdx) break;
        currIdx = keyIdx + 1;
        
        // read name and value
        char* keyName = ReadName(config, currIdx);
        currIdx += strlen(keyName) + 1;

        char* strValue = ReadName(config, currIdx);
        double value = StrToDouble(strValue);

        // super ugly
        if (strcmp(keyName, "col") == 0) {color = HexToRGB(value); continue; }

        if (strcmp(keyName, "sma") == 0) { orbit.smaxis = value; continue; }
        if (strcmp(keyName, "lpe") == 0) { orbit.lpe = value; continue; }
        if (strcmp(keyName, "lan") == 0) { orbit.lan = value; continue; }
        if (strcmp(keyName, "mna") == 0) { orbit.mna = value; continue; }
        if (strcmp(keyName, "ecc") == 0) { orbit.eccentricity = value; continue; }
        if (strcmp(keyName, "inc") == 0) { orbit.inclination = value; continue; }
    }

    return CreatePlanet(&orbit, &color, name);
}

void InitScene(Scene* scene, char* system) {
    (void)scene;
    char* contents = ReadSystemsFile();
    char* systemConfig = GetSystemConfig(contents, system);
    if (strcmp(systemConfig, "") == 0) return;
    
    int currIdx = 0;
    while (true) {
        int planetStartIdx = FindNextChar(systemConfig, '-', currIdx);
        if (planetStartIdx == -1) break;
        currIdx = planetStartIdx + 1;

        // get name
        char* planetName = ReadName(systemConfig, currIdx);
        currIdx += strlen(planetName);
    
        // get end idx of the planet config, then make it and add to scene
        int nextPlanetIdx = FindNextChar(systemConfig, '-', currIdx);
        nextPlanetIdx = (nextPlanetIdx == -1) ? (int)strlen(systemConfig) - 1 : nextPlanetIdx;

        Planet planet = MakePlanetFromConfig(planetName, systemConfig, currIdx, nextPlanetIdx);
        AddToScene(scene, &planet);
    }
}
