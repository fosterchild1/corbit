#ifndef PLANET_H
#define PLANET_H

#include <stdint.h>

typedef struct {
    double smaxis;
    double lpe; // longitude of periapsis
    double lan; // longitude of ascending node
    double mna; // mean anomaly at epoch
    float eccentricity;
    float inclination;
} OrbitParams;

typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} Color;

typedef struct {
    OrbitParams orbitparams;
    Color color;
} Planet;

#endif
