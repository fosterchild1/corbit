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
    
    string[fileSize] = '\0';
    return string;
}

char* ReadSystemsFile(void) {
    // get file directory
    char* homeDir = getenv("HOME");
    int dirLen = strlen(homeDir) + strlen(systemsDir);
    char fileDir[dirLen + 1];
    strcpy(fileDir, homeDir);
    strncat(fileDir, systemsDir, dirLen);
    fileDir[dirLen] = '\0';
    
    return ReadFile(fileDir);
}

int FindNextChar(char* str, char ch, int startIdx, int len) {
    if (startIdx + 1 > len) return -1;
    char* p = str + startIdx;

    while (*p != '\0') {
        if (*p == ch) return p - str;
        p++;
    }

    return -1;
}

char* ReadName(char* str, int startIdx, int len) {
    if (startIdx + 1 > len) return "";
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

void HandleOrbitProperty(OrbitParams* orbit, char* keyName, double value) {
    // very ugly...
    if (strcmp(keyName, "sma") == 0) { orbit->smaxis = value; }
    if (strcmp(keyName, "lpe") == 0) { orbit->lpe = value; }
    if (strcmp(keyName, "lan") == 0) { orbit->lan = value; }
    if (strcmp(keyName, "mna") == 0) { orbit->mna = value; }
    if (strcmp(keyName, "ecc") == 0) { orbit->eccentricity = value; }
    if (strcmp(keyName, "inc") == 0) { orbit->inclination = value; }

    if (strcmp(keyName, "") != 0) free(keyName); // free only if malloc'd
}

char* GetSystemConfig(char* contents, char* system) {
    int currIdx = 0;
    int contentsLen = strlen(contents);

    // find system, denoted by .systemname
    while (true) {
        int nameStartIdx = FindNextChar(contents, '.', currIdx, contentsLen);
        if (nameStartIdx == -1) return "";
        currIdx  = nameStartIdx + 1;

        char* systemName = ReadName(contents, currIdx, contentsLen);
        currIdx += strlen(systemName);

        int isSameSystem = strcmp(systemName, system) == 0; free(systemName);
        if (!isSameSystem) continue;
        break;
    }
    
    // find where the system config ends, either from the next system or by end of file
    int nextSystemIdx = FindNextChar(contents, '.', currIdx, contentsLen);
    nextSystemIdx = (nextSystemIdx == -1) ? contentsLen - 1 : nextSystemIdx;

    return Strsub(contents, currIdx, nextSystemIdx);
}

Planet MakePlanetFromConfig(char* name, char* config, int startIdx, int endIdx) {
    OrbitParams orbit = {0, 0, 0, 0, 0, 0};
    Color color = {255, 255, 255, 0};

    int currIdx = startIdx;
    while (true) {
        // all variable names start with #, followed by their value
        int keyIdx = FindNextChar(config, '#', currIdx, endIdx);
        if (keyIdx == -1 || keyIdx > endIdx) break;
        currIdx = keyIdx + 1;
        
        // read name and value
        char* keyName = ReadName(config, currIdx, endIdx);
        currIdx += strlen(keyName) + 1;

        char* strValue = ReadName(config, currIdx, endIdx);
        double value = StrToDouble(strValue); 
        if (strcmp(strValue, "") != 0) free(strValue); // free strValue if succesfully malloc'd

        // super ugly
        if (strcmp(keyName, "col") == 0) {color = HexToRGB(value); free(keyName); continue; }
        HandleOrbitProperty(&orbit, keyName, value);
    }

    return CreatePlanet(&orbit, &color, name);
}

void InitScene(Scene* scene, char* system) {
    (void)scene;
    char* contents = ReadSystemsFile();
    char* systemConfig = GetSystemConfig(contents, system);
    if (strcmp(systemConfig, "") == 0) return;
    
    int currIdx = 0;
    int configLen = strlen(systemConfig);

    while (true) {
        int planetStartIdx = FindNextChar(systemConfig, '-', currIdx, configLen);
        if (planetStartIdx == -1) break;
        currIdx = planetStartIdx + 1;

        // get name
        char* planetName = ReadName(systemConfig, currIdx, configLen);
        currIdx += strlen(planetName);
    
        // get end idx of the planet config, then make it and add to scene
        int nextPlanetIdx = FindNextChar(systemConfig, '-', currIdx, configLen);
        nextPlanetIdx = (nextPlanetIdx == -1) ? configLen - 1 : nextPlanetIdx;

        Planet planet = MakePlanetFromConfig(planetName, systemConfig, currIdx, nextPlanetIdx);
        AddToScene(scene, &planet);
    }

    free(contents); free(systemConfig);
}
