// number.c
#include "types/number.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

BigInt createBigInt(const char *str)
{
    BigInt result;
    result.length = strlen(str);
    result.isNegative = (str[0] == '-');
    result.digits = (char *)malloc(result.length + 1);
    strcpy(result.digits, str + result.isNegative);
    return result;
}

void freeBigInt(BigInt n)
{
    free(n.digits);
}

static void trimLeadingZeros(BigInt *n)
{
    int start = 0;
    while (start < n->length - 1 && n->digits[start] == '0')
    {
        start++;
    }
    if (start > 0)
    {
        memmove(n->digits, n->digits + start, n->length - start + 1);
        n->length -= start;
    }
}

static int compareBigInt(BigInt a, BigInt b)
{
    if (a.length > b.length)
        return 1;
    if (a.length < b.length)
        return -1;
    return strcmp(a.digits, b.digits);
}

Number createNumberFromString(const char *str)
{
    Number result;
    char *endptr;
    int value = strtol(str, &endptr, 10);

    if (*endptr == '\0' && (value != LONG_MAX && value != LONG_MIN))
    {
        result.isBig = false;
        result.smallInt = value;
    }
    else
    {
        result.isBig = true;
        result.bigInt = createBigInt(str);
    }
    return result;
}

Number createNumberFromInt(int value)
{
    Number result;
    result.isBig = false;
    result.smallInt = value;
    return result;
}

void printNumber(Number n)
{
    if (n.isBig)
    {
        if (n.bigInt.isNegative)
        {
            printf("-");
        }
        printf("%s\n", n.bigInt.digits);
    }
    else
    {
        printf("%d\n", n.smallInt);
    }
}

BigInt addBigInt(BigInt a, BigInt b)
{
    if (a.isNegative != b.isNegative)
    {
        // Handle addition of positive and negative BigInts
        if (a.isNegative)
        {
            a.isNegative = false;
            return subBigInt(b, a);
        }
        else
        {
            b.isNegative = false;
            return subBigInt(a, b);
        }
    }

    int maxLength = (a.length > b.length) ? a.length : b.length;
    char *result = (char *)malloc(maxLength + 2);

    int carry = 0;
    int i;

    for (i = 0; i < maxLength; i++)
    {
        int digitA = (i < a.length) ? a.digits[a.length - 1 - i] - '0' : 0;
        int digitB = (i < b.length) ? b.digits[b.length - 1 - i] - '0' : 0;
        int sum = digitA + digitB + carry;
        result[maxLength - i] = (sum % 10) + '0';
        carry = sum / 10;
    }

    if (carry)
    {
        result[0] = carry + '0';
        result[maxLength + 1] = '\0';
    }
    else
    {
        strcpy(result, result + 1);
    }

    BigInt sumResult;
    sumResult.length = strlen(result);
    sumResult.digits = result;
    sumResult.isNegative = a.isNegative;

    trimLeadingZeros(&sumResult);

    return sumResult;
}

BigInt subBigInt(BigInt a, BigInt b)
{
    if (a.isNegative != b.isNegative)
    {
        b.isNegative = !b.isNegative;
        return addBigInt(a, b);
    }

    // Ensure a >= b
    if (compareBigInt(a, b) < 0)
    {
        BigInt temp = a;
        a = b;
        b = temp;
        a.isNegative = !a.isNegative;
        b.isNegative = !b.isNegative;
    }

    int maxLength = (a.length > b.length) ? a.length : b.length;
    char *result = (char *)malloc(maxLength + 1);

    int borrow = 0;
    int i;

    for (i = 0; i < maxLength; i++)
    {
        int digitA = (i < a.length) ? a.digits[a.length - 1 - i] - '0' : 0;
        int digitB = (i < b.length) ? b.digits[b.length - 1 - i] - '0' : 0;
        int diff = digitA - digitB - borrow;
        if (diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        result[maxLength - i - 1] = diff + '0';
    }

    BigInt subResult;
    subResult.length = maxLength;
    subResult.digits = (char *)malloc(subResult.length + 1);
    strncpy(subResult.digits, result, subResult.length);
    subResult.digits[subResult.length] = '\0';
    subResult.isNegative = a.isNegative;

    trimLeadingZeros(&subResult);

    free(result);

    return subResult;
}

BigInt mulBigInt(BigInt a, BigInt b)
{
    int maxLength = a.length + b.length;
    char *result = (char *)calloc(maxLength + 1, sizeof(char));

    for (int i = 0; i < a.length; i++)
    {
        int carry = 0;
        for (int j = 0; j < b.length; j++)
        {
            int product = (a.digits[a.length - 1 - i] - '0') * (b.digits[b.length - 1 - j] - '0') + carry + (result[maxLength - 1 - i - j] - '0');
            result[maxLength - 1 - i - j] = (product % 10) + '0';
            carry = product / 10;
        }
        result[maxLength - 1 - i - b.length] += carry;
    }

    BigInt mulResult;
    mulResult.length = maxLength;
    mulResult.digits = (char *)malloc(mulResult.length + 1);
    strncpy(mulResult.digits, result, mulResult.length);
    mulResult.digits[mulResult.length] = '\0';
    mulResult.isNegative = a.isNegative != b.isNegative;

    trimLeadingZeros(&mulResult);

    free(result);

    return mulResult;
}

BigInt divBigInt(BigInt a, BigInt b)
{
    // This function will handle division.
    // Note: This implementation assumes positive numbers and no leading zeros.
    BigInt result;

    if (b.length == 1 && b.digits[0] == '0')
    {
        printf("Error: Division by zero\n");
        result.length = 0;
        result.digits = NULL;
        result.isNegative = false;
        return result;
    }

    // Initialize result
    result.length = a.length;
    result.digits = (char *)malloc(result.length + 1);
    memset(result.digits, '0', result.length);
    result.digits[result.length] = '\0';
    result.isNegative = a.isNegative != b.isNegative;

    BigInt remainder = a;
    remainder.isNegative = false;

    BigInt divisor = b;
    divisor.isNegative = false;

    for (int i = 0; i < a.length; i++)
    {
        // Shift the remainder and get the current digit
        remainder.length--;
        if (remainder.length > 0)
        {
            remainder.digits = (char *)realloc(remainder.digits, remainder.length + 1);
            memmove(remainder.digits, remainder.digits + 1, remainder.length);
        }
        else
        {
            remainder.digits = (char *)realloc(remainder.digits, 1);
        }
        remainder.digits[remainder.length - 1] = a.digits[i];

        // Find the quotient digit
        int quotientDigit = 0;
        while (compareBigInt(remainder, divisor) >= 0)
        {
            remainder = subBigInt(remainder, divisor);
            quotientDigit++;
        }

        result.digits[i] = quotientDigit + '0';
    }

    trimLeadingZeros(&result);

    return result;
}

Number addNumber(Number a, Number b)
{
    Number result;

    if (!a.isBig && !b.isBig)
    {
        result.isBig = false;
        result.smallInt = a.smallInt + b.smallInt;
    }
    else
    {
        if (!a.isBig)
        {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%d", a.smallInt);
            a = createNumberFromString(bufA);
        }
        if (!b.isBig)
        {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%d", b.smallInt);
            b = createNumberFromString(bufB);
        }

        result.isBig = true;
        result.bigInt = addBigInt(a.bigInt, b.bigInt);
    }

    return result;
}

Number subNumber(Number a, Number b)
{
    Number result;

    if (!a.isBig && !b.isBig)
    {
        result.isBig = false;
        result.smallInt = a.smallInt - b.smallInt;
    }
    else
    {
        if (!a.isBig)
        {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%d", a.smallInt);
            a = createNumberFromString(bufA);
        }
        if (!b.isBig)
        {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%d", b.smallInt);
            b = createNumberFromString(bufB);
        }

        result.isBig = true;
        result.bigInt = subBigInt(a.bigInt, b.bigInt);
    }

    return result;
}

Number mulNumber(Number a, Number b)
{
    Number result;

    if (!a.isBig && !b.isBig)
    {
        result.isBig = false;
        result.smallInt = a.smallInt * b.smallInt;
    }
    else
    {
        if (!a.isBig)
        {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%d", a.smallInt);
            a = createNumberFromString(bufA);
        }
        if (!b.isBig)
        {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%d", b.smallInt);
            b = createNumberFromString(bufB);
        }

        result.isBig = true;
        result.bigInt = mulBigInt(a.bigInt, b.bigInt);
    }

    return result;
}

Number divNumber(Number a, Number b)
{
    Number result;

    if (!a.isBig && !b.isBig)
    {
        if (b.smallInt != 0)
        {
            result.isBig = false;
            result.smallInt = a.smallInt / b.smallInt;
        }
        else
        {
            printf("Error: Division by zero\n");
            result.isBig = false;
            result.smallInt = 0;
        }
    }
    else
    {
        if (!a.isBig)
        {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%d", a.smallInt);
            a = createNumberFromString(bufA);
        }
        if (!b.isBig)
        {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%d", b.smallInt);
            b = createNumberFromString(bufB);
        }

        result.isBig = true;
        result.bigInt = divBigInt(a.bigInt, b.bigInt);
    }

    return result;
}

void freeNumber(Number n)
{
    if (n.isBig)
    {
        freeBigInt(n.bigInt);
    }
}
