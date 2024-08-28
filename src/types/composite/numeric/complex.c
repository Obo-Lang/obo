// complex.c
#include "types/composite/numeric/complex.h"

#include <stdio.h>
#include <stdlib.h>

Complex createComplex(Real real, Real imag) {
    Complex z;
    z.real = real;
    z.imag = imag;
    return z;
}

Complex createComplexFromString(const char *realStr, const char *imagStr) {
    Complex z;
    z.real = createRealFromString(realStr);
    z.imag = createRealFromString(imagStr);
    return z;
}

void printComplex(Complex z) {
    printReal(z.real);
    if (z.imag.isBig || z.imag.smallFloat >= 0) {
        printf(" + ");
    }
    printReal(z.imag);
    printf("i\n");
}

Complex addComplex(Complex a, Complex b) {
    Complex result;
    result.real = addReal(a.real, b.real);
    result.imag = addReal(a.imag, b.imag);
    return result;
}

Complex mulComplex(Complex a, Complex b) {
    Complex result;

    Real realPart1 = mulReal(a.real, b.real);
    Real realPart2 = mulReal(a.imag, b.imag);
    result.real = subReal(realPart1, realPart2);

    Real imagPart1 = mulReal(a.real, b.imag);
    Real imagPart2 = mulReal(a.imag, b.real);
    result.imag = addReal(imagPart1, imagPart2);

    freeReal(realPart1);
    freeReal(realPart2);
    freeReal(imagPart1);
    freeReal(imagPart2);

    return result;
}

void freeComplex(Complex z) {
    freeReal(z.real);
    freeReal(z.imag);
}
