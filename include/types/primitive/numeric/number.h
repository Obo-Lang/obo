// number.h
#pragma once

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

BigInt createBigInt(const char *str);
BigInt addBigInt(BigInt a, BigInt b);
BigInt subBigInt(BigInt a, BigInt b);
BigInt mulBigInt(BigInt a, BigInt b);
BigInt divBigInt(BigInt a, BigInt b);
void freeBigInt(BigInt n);

Number createNumberFromString(const char *str);
Number createNumberFromInt(int value);
Number addNumber(Number a, Number b);
Number subNumber(Number a, Number b);
Number mulNumber(Number a, Number b);
Number divNumber(Number a, Number b);
void printNumber(Number n);
void freeNumber(Number n);

#endif // NUMBER_H
