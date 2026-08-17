#pragma once

#include <stdlib.h>
#include "Simulation/scene.h"

#define TERM_FONT_RATIO 1.5 // fonts are taller than wide

#define M_PI 3.14159265358979323846
#define M_TAU 2*M_PI

#define max(a, b) ((a) > (b) ? (a) : (b))
#define rad(x) ((x) * M_PI / 180.0)

#define DimColor(c) (((c) > 35) ? ((c) - 35) : 0)

float CalculateEccentricAnomaly(double mna, float ecc);

FPoint3 GetPointOnElipse(float xLocal, float yLocal, float trigArr[6]);

char* Strsub(char* str, int startIdx, int endIdx);

void* Safemalloc(size_t size);

Color HexToRGB(int hex);

void InitDefaultColorPairs(void);
