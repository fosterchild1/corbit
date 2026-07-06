#ifndef UTIL_H
#define UTIL_H

#include "simulation.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

float CalculateEccentricAnomaly(double mna, float ecc);

FPoint3 GetPointOnElipse(float xLocal, float yLocal, float trigArr[6]);

char* Strsub(char* str, int startIdx, int endIdx);

int StrToInt(char* str);

double StrToDouble(char* str);

Color HexToRGB(int hex);

#endif
