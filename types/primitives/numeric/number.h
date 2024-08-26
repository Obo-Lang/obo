// number.h

#ifndef NUMBER_H
#define NUMBER_H

#include <stdbool.h>

typedef struct {
    char *digits;
    int length;
    bool isNegative;
} BigInt;

typedef struct {
    bool isBig;
    union {
        int smallInt;
        BigInt bigInt;
    };
} Number;

Number createNumberFromString(const char *str);

Number createNumberFromInt(int value);

void printNumber(Number n);

Number addNumber(Number a, Number b);

Number subNumber(Number a, Number b);

Number mulNumber(Number a, Number b);

Number divNumber(Number a, Number b);

void freeNumber(Number n);

#endif
