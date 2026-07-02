#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "simulation.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

float CalculateEccentricAnomaly(double mna, float ecc);

FPoint3 GetPointOnElipse(float xLocal, float yLocal, float trigArr[6]);


#endif
