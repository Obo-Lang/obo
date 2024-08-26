// real.c

#include "real.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>

BigFloat createBigFloat(const char *str) {
    BigFloat result;
    result.length = strlen(str);
    result.digits = (char *)malloc(result.length + 1);
    strcpy(result.digits, str);
    
    char *dot = strchr(str, '.');
    if (dot) {
        result.decimalPosition = dot - str;
    } else {
        result.decimalPosition = result.length;
    }
    
    return result;
}

void freeBigFloat(BigFloat n) {
    free(n.digits);
}

Real createRealFromString(const char *str) {
    Real result;

    char *endptr;
    double value = strtod(str, &endptr);

    if (*endptr == '\0' && value >= -DBL_MAX && value <= DBL_MAX) {
        result.isBig = false;
        result.smallFloat = value;
    } else {
        result.isBig = true;
        result.bigFloat = createBigFloat(str);
    }

    return result;
}

Real createRealFromDouble(double value) {
    Real result;
    result.isBig = false;
    result.smallFloat = value;
    return result;
}

void printReal(Real n) {
    if (n.isBig) {
        printf("%s\n", n.bigFloat.digits);
    } else {
        printf("%lf\n", n.smallFloat);
    }
}

BigFloat addBigFloat(BigFloat a, BigFloat b) {
    int maxLength = (a.length > b.length) ? a.length : b.length;
    char *result = (char *)malloc(maxLength + 2);

    int carry = 0, i = 0;
    int decimalPosition = a.decimalPosition;

    for (i = 0; i < maxLength; i++) {
        int digitA = (i < a.length) ? a.digits[a.length - 1 - i] - '0' : 0;
        int digitB = (i < b.length) ? b.digits[b.length - 1 - i] - '0' : 0;
        if (a.length - 1 - i == decimalPosition - 1) {
            result[maxLength - i] = '.';
        } else {
            int sum = digitA + digitB + carry;
            result[maxLength - i] = (sum % 10) + '0';
            carry = sum / 10;
        }
    }

    if (carry) {
        result[0] = carry + '0';
        result[maxLength + 1] = '\0';
    } else {
        strcpy(result, result + 1);
    }

    BigFloat sumResult;
    sumResult.length = strlen(result);
    sumResult.digits = result;
    sumResult.decimalPosition = decimalPosition;

    return sumResult;
}

BigFloat subBigFloat(BigFloat a, BigFloat b) {
    BigFloat result;
    // TODO
    return result;
}

Real addReal(Real a, Real b) {
    Real result;

    if (!a.isBig && !b.isBig) {
        double sum = a.smallFloat + b.smallFloat;

        if (sum == HUGE_VAL || sum == -HUGE_VAL) {
            char bufA[50], bufB[50];
            snprintf(bufA, sizeof(bufA), "%lf", a.smallFloat);
            snprintf(bufB, sizeof(bufB), "%lf", b.smallFloat);

            BigFloat bigA = createBigFloat(bufA);
            BigFloat bigB = createBigFloat(bufB);
            result.isBig = true;
            result.bigFloat = addBigFloat(bigA, bigB);
            freeBigFloat(bigA);
            freeBigFloat(bigB);
        } else {
            result.isBig = false;
            result.smallFloat = sum;
        }
    } else {
        if (!a.isBig) {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%lf", a.smallFloat);
            a = createRealFromString(bufA);
        }
        if (!b.isBig) {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%lf", b.smallFloat);
            b = createRealFromString(bufB);
        }

        result.isBig = true;
        result.bigFloat = addBigFloat(a.bigFloat, b.bigFloat);
    }

    return result;
}

Real subReal(Real a, Real b) {
    Real result;
    // TODO
    return result;
}

Real mulReal(Real a, Real b) {
    Real result;
    // TODO
    return result;
}

Real divReal(Real a, Real b) {
    Real result;
    // TODO
    return result;
}

void freeReal(Real n) {
    if (n.isBig) {
        freeBigFloat(n.bigFloat);
    }
}
