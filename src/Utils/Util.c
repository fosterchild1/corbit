#include <math.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Simulation/scene.h"
#include "Utils/util.h"

float CalculateEccentricAnomaly(double mna, float ecc) {
    // use newton-raphson to approximate E.
    // f(x) = E - e*sin E - M
    // f'(x) = 1 - e*cos E
    float E = mna;

    for (int i = 0; i < 5; i++) {
        float f = E - ecc * sinf(E) - mna;
        float fPrime = 1 - ecc * cosf(E);

        E -= f/fPrime;
    }
    
    return E;
}

FPoint3 GetPointOnElipse(float xLocal, float yLocal, const float trigArr[6]) {
    float sinOmega = trigArr[0]; float cosOmega = trigArr[1];
    float sinLan = trigArr[2]; float cosLan = trigArr[3];
    float sinInc = trigArr[4]; float cosInc = trigArr[5];
    
    // apply transposed rotation matrix. i wont act like i understand this
    float x = xLocal * (cosOmega * cosLan - sinOmega * sinLan * cosInc) - yLocal * (sinOmega * cosLan + cosOmega * sinLan * cosInc);
    float y = xLocal * (cosOmega * sinLan + sinOmega * cosLan * cosInc) - yLocal * (sinOmega * sinLan - cosOmega * cosLan * cosInc);
    float z = xLocal * (sinOmega * sinInc)                              + yLocal * (cosOmega * sinInc);

    return (FPoint3){x, y, z};
}

int RandInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void* Safemalloc(size_t size) {
    void* p = malloc(size);
    if (p == NULL) {
        exit(EXIT_FAILURE);
    }

    return p;
}

char* Strsub(const char* str, int startIdx, int endIdx) {
    if (endIdx <= startIdx) return "";

    int len = endIdx - startIdx;
    char* result = Safemalloc((len + 1) * sizeof(char));
    strncpy(result, str + startIdx, len);
    result[len] = '\0';

    return result;
}

Color HexToRGB(int hex) {
    int r = (hex >> 16) & 0xFF;
    int g = (hex >> 8) & 0xFF;
    int b = hex & 0xFF;
    return (Color){r, g, b, 0};
}

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

void InitDefaultColorPairs(void) {
    for (short i = 0; i < 16; i++) {
        init_pair(i, i, -1);
    }
}
