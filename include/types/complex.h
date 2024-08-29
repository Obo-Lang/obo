// complex.h
#pragma once

#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdbool.h>
#include "types/real.h"

typedef struct
{
    Real real;
    Real imag;
} Complex;

Complex createComplex(Real real, Real imag);
Complex createComplexFromString(const char *realStr, const char *imagStr);
Complex addComplex(Complex a, Complex b);
Complex mulComplex(Complex a, Complex b);
void printComplex(Complex z);
void freeComplex(Complex z);

#endif
