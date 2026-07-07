#include <math.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "simulation.h"

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

FPoint3 GetPointOnElipse(float xLocal, float yLocal, float trigArr[6]) {
    float sinOmega = trigArr[0]; float cosOmega = trigArr[1];
    float sinLan = trigArr[2]; float cosLan = trigArr[3];
    float sinInc = trigArr[4]; float cosInc = trigArr[5];
    
    // apply transposed rotation matrix. i wont act like i understand this
    float x = xLocal * (cosOmega * cosLan - sinOmega * sinLan * cosInc) - yLocal * (sinOmega * cosLan + cosOmega * sinLan * cosInc);
    float y = xLocal * (cosOmega * sinLan + sinOmega * cosLan * cosInc) - yLocal * (sinOmega * sinLan - cosOmega * cosLan * cosInc);
    float z = xLocal * (sinOmega * sinInc)                              + yLocal * (cosOmega * sinInc);

    return (FPoint3){x, y, z};
}

char* Strsub(char* str, int startIdx, int endIdx) {
    if (endIdx <= startIdx) return "";

    int len = endIdx - startIdx;
    char* result = malloc((len + 1) * sizeof(char));
    strncpy(result, str + startIdx, len);
    result[len] = '\0';

    return result;
}

int StrToInt(char* str) {
    int num = 0;
    char* p = str;

    // handle sign
    bool neg = false;
    if (*p == '-') { neg = true; p++; }
    
    while (*p != '\0') {
        if (!(*p >= '0' && *p <= '9')) return 0;
        
        num = num * 10 + (*p - '0');
        p++;
    }
        
    return neg ? -num : num;
}

double StrToDouble(char* str) {
    double num = 0;
    char* p = str;

    // handle sign
    bool neg = false;
    if (*p == '-') { neg = true; p++; }

    bool fraction = false;
    int fractionAmt = 1;

    while (*p != '\0') {
        char c = *p;
        if (c == '.' || c == ',') { fraction = true; p++; continue; }

        if (!(c >= '0' && c <= '9') && (c != '.' && c != ',')) return 0;
        
        if (fraction) {
            num += (c - '0') / pow(10, fractionAmt);
            fractionAmt++;
        } else
            num = num * 10 + (c - '0'); 

        p++;
    }

    return neg ? -num : num;
}

Color HexToRGB(int hex) {
    int r = (hex >> 16) & 0xFF;
    int g = (hex >> 8) & 0xFF;
    int b = hex & 0xFF;
    return (Color){r, g, b, 0};
}

void InitDefaultColorPairs(void) {
    for (short i = 0; i < 16; i++) {
        init_pair(i, i, -1);
    }
}
