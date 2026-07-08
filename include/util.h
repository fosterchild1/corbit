#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include "simulation.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define rad(x) ((x) * M_PI / 180.0)

float CalculateEccentricAnomaly(double mna, float ecc);

FPoint3 GetPointOnElipse(float xLocal, float yLocal, float trigArr[6]);

char* Strsub(char* str, int startIdx, int endIdx);

int StrToInt(char* str);

double StrToDouble(char* str);

void* Safemalloc(size_t size);

Color HexToRGB(int hex);

void InitDefaultColorPairs(void);

#endif
