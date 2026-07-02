#include <math.h>
#include "../include/simulation.h"

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

    // thanks superchil for this monstrosity
    float x = xLocal * (cosOmega * cosLan - sinOmega * sinLan * cosInc) - yLocal * (sinOmega * cosLan + cosOmega * sinLan * cosInc);
    float y = xLocal * (cosOmega * sinLan + sinOmega * cosLan * cosInc) - yLocal * (sinOmega * sinLan - cosOmega * cosLan * cosInc);
    float z = xLocal * (sinOmega * sinInc)                                    + yLocal * (cosOmega * sinInc);

    return (FPoint3){x, y, z};
}

void* max(void* a, void* b) {
    return (a > b) ? a : b;
}
