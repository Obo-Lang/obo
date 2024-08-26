// complex.h

#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdbool.h>
#include "real.h"

typedef struct {
    Real real;
    Real imag;
} Complex;

Complex createComplex(Real real, Real imag);

Complex createComplexFromString(const char *realStr, const char *imagStr);

void printComplex(Complex z);

Complex addComplex(Complex a, Complex b);

Complex mulComplex(Complex a, Complex b);

void freeComplex(Complex z);

#endif
