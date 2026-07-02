#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "simulation.h"

float CalculateEccentricAnomaly(double mna, float ecc);

FPoint3 GetPointOnElipse(float xLocal, float yLocal, float trigArr[6]);

#endif
