// real.h

#ifndef REAL_H
#define REAL_H

#include <stdbool.h>

typedef struct {
    char *digits;
    int length;
    int decimalPosition;
} BigFloat;

typedef struct {
    bool isBig;
    union {
        double smallFloat;
        BigFloat bigFloat;
    };
} Real;

Real createRealFromString(const char *str);

Real createRealFromDouble(double value);

void printReal(Real n);

Real addReal(Real a, Real b);

Real subReal(Real a, Real b);

Real mulReal(Real a, Real b);

Real divReal(Real a, Real b);

void freeReal(Real n);

#endif
