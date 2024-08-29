// real.c
#include "types/real.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>

BigFloat createBigFloat(const char *str)
{
    BigFloat result;
    result.length = strlen(str);
    result.digits = (char *)malloc(result.length + 1);
    strcpy(result.digits, str);

    char *dot = strchr(str, '.');
    if (dot)
    {
        result.decimalPosition = dot - str;
    }
    else
    {
        result.decimalPosition = result.length;
    }

    return result;
}

void freeBigFloat(BigFloat n)
{
    free(n.digits);
}

Real createRealFromString(const char *str)
{
    Real result;

    char *endptr;
    double value = strtod(str, &endptr);

    if (*endptr == '\0' && value >= -DBL_MAX && value <= DBL_MAX)
    {
        result.isBig = false;
        result.smallFloat = value;
    }
    else
    {
        result.isBig = true;
        result.bigFloat = createBigFloat(str);
    }

    return result;
}

Real createRealFromDouble(double value)
{
    Real result;
    result.isBig = false;
    result.smallFloat = value;
    return result;
}

void printReal(Real n)
{
    if (n.isBig)
    {
        printf("%s\n", n.bigFloat.digits);
    }
    else
    {
        printf("%lf\n", n.smallFloat);
    }
}

BigFloat addBigFloat(BigFloat a, BigFloat b)
{
    int maxLength = (a.length > b.length) ? a.length : b.length;
    char *result = (char *)malloc(maxLength + 2);

    int carry = 0, i = 0;
    int decimalPosition = a.decimalPosition;

    for (i = 0; i < maxLength; i++)
    {
        int digitA = (i < a.length) ? a.digits[a.length - 1 - i] - '0' : 0;
        int digitB = (i < b.length) ? b.digits[b.length - 1 - i] - '0' : 0;
        if (a.length - 1 - i == decimalPosition - 1)
        {
            result[maxLength - i] = '.';
        }
        else
        {
            int sum = digitA + digitB + carry;
            result[maxLength - i] = (sum % 10) + '0';
            carry = sum / 10;
        }
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

    BigFloat sumResult;
    sumResult.length = strlen(result);
    sumResult.digits = result;
    sumResult.decimalPosition = decimalPosition;

    return sumResult;
}

BigFloat subBigFloat(BigFloat a, BigFloat b)
{
    int maxLength = (a.length > b.length) ? a.length : b.length;
    char *result = (char *)malloc(maxLength + 1);

    int borrow = 0, i = 0;
    int decimalPosition = a.decimalPosition;

    for (i = 0; i < maxLength; i++)
    {
        int digitA = (i < a.length) ? a.digits[a.length - 1 - i] - '0' : 0;
        int digitB = (i < b.length) ? b.digits[b.length - 1 - i] - '0' : 0;
        if (a.length - 1 - i == decimalPosition - 1)
        {
            result[maxLength - i] = '.';
        }
        else
        {
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
    }

    int start = 0;
    while (start < maxLength - 1 && result[start] == '0')
    {
        start++;
    }

    BigFloat subResult;
    subResult.length = maxLength - start;
    subResult.digits = (char *)malloc(subResult.length + 1);
    strncpy(subResult.digits, result + start, subResult.length);
    subResult.digits[subResult.length] = '\0';
    subResult.decimalPosition = decimalPosition;

    free(result);

    return subResult;
}

BigFloat mulBigFloat(BigFloat a, BigFloat b)
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

    int decimalPosition = a.decimalPosition + b.decimalPosition - 1;

    int start = 0;
    while (start < maxLength - 1 && result[start] == '0')
    {
        start++;
    }

    BigFloat mulResult;
    mulResult.length = maxLength - start;
    mulResult.digits = (char *)malloc(mulResult.length + 1);
    strncpy(mulResult.digits, result + start, mulResult.length);
    mulResult.digits[mulResult.length] = '\0';
    mulResult.decimalPosition = decimalPosition;

    free(result);

    return mulResult;
}

BigFloat divBigFloat(BigFloat a, BigFloat b)
{
    int maxLength = a.length;
    char *result = (char *)malloc(maxLength + 1);
    char *remainder = (char *)calloc(maxLength + 1, sizeof(char));

    strncpy(remainder, a.digits, a.length);

    for (int i = 0; i < maxLength; i++)
    {
        int digitA = remainder[i] - '0';
        int digitB = (b.length > i) ? b.digits[i] - '0' : 0;

        if (digitB == 0)
        {
            result[i] = '0';
        }
        else
        {
            result[i] = (digitA / digitB) + '0';
            remainder[i + 1] = (digitA % digitB) + '0';
        }
    }

    BigFloat divResult;
    divResult.length = strlen(result);
    divResult.digits = result;
    divResult.decimalPosition = a.decimalPosition - b.decimalPosition;

    free(remainder);

    return divResult;
}

Real addReal(Real a, Real b)
{
    Real result;

    if (!a.isBig && !b.isBig)
    {
        double sum = a.smallFloat + b.smallFloat;

        if (sum == HUGE_VAL || sum == -HUGE_VAL)
        {
            char bufA[50], bufB[50];
            snprintf(bufA, sizeof(bufA), "%lf", a.smallFloat);
            snprintf(bufB, sizeof(bufB), "%lf", b.smallFloat);

            BigFloat bigA = createBigFloat(bufA);
            BigFloat bigB = createBigFloat(bufB);
            result.isBig = true;
            result.bigFloat = addBigFloat(bigA, bigB);
            freeBigFloat(bigA);
            freeBigFloat(bigB);
        }
        else
        {
            result.isBig = false;
            result.smallFloat = sum;
        }
    }
    else
    {
        if (!a.isBig)
        {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%lf", a.smallFloat);
            a = createRealFromString(bufA);
        }
        if (!b.isBig)
        {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%lf", b.smallFloat);
            b = createRealFromString(bufB);
        }

        result.isBig = true;
        result.bigFloat = addBigFloat(a.bigFloat, b.bigFloat);
    }

    return result;
}

Real subReal(Real a, Real b)
{
    Real result;

    if (!a.isBig && !b.isBig)
    {
        double diff = a.smallFloat - b.smallFloat;

        if (diff == HUGE_VAL || diff == -HUGE_VAL)
        {
            char bufA[50], bufB[50];
            snprintf(bufA, sizeof(bufA), "%lf", a.smallFloat);
            snprintf(bufB, sizeof(bufB), "%lf", b.smallFloat);

            BigFloat bigA = createBigFloat(bufA);
            BigFloat bigB = createBigFloat(bufB);
            result.isBig = true;
            result.bigFloat = subBigFloat(bigA, bigB);
            freeBigFloat(bigA);
            freeBigFloat(bigB);
        }
        else
        {
            result.isBig = false;
            result.smallFloat = diff;
        }
    }
    else
    {
        if (!a.isBig)
        {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%lf", a.smallFloat);
            a = createRealFromString(bufA);
        }
        if (!b.isBig)
        {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%lf", b.smallFloat);
            b = createRealFromString(bufB);
        }

        result.isBig = true;
        result.bigFloat = subBigFloat(a.bigFloat, b.bigFloat);
    }

    return result;
}

Real mulReal(Real a, Real b)
{
    Real result;

    if (!a.isBig && !b.isBig)
    {
        double product = a.smallFloat * b.smallFloat;

        if (product == HUGE_VAL || product == -HUGE_VAL)
        {
            char bufA[50], bufB[50];
            snprintf(bufA, sizeof(bufA), "%lf", a.smallFloat);
            snprintf(bufB, sizeof(bufB), "%lf", b.smallFloat);

            BigFloat bigA = createBigFloat(bufA);
            BigFloat bigB = createBigFloat(bufB);
            result.isBig = true;
            result.bigFloat = mulBigFloat(bigA, bigB);
            freeBigFloat(bigA);
            freeBigFloat(bigB);
        }
        else
        {
            result.isBig = false;
            result.smallFloat = product;
        }
    }
    else
    {
        if (!a.isBig)
        {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%lf", a.smallFloat);
            a = createRealFromString(bufA);
        }
        if (!b.isBig)
        {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%lf", b.smallFloat);
            b = createRealFromString(bufB);
        }

        result.isBig = true;
        result.bigFloat = mulBigFloat(a.bigFloat, b.bigFloat);
    }

    return result;
}

Real divReal(Real a, Real b)
{
    Real result;

    if (!a.isBig && !b.isBig)
    {
        if (b.smallFloat != 0)
        {
            result.isBig = false;
            result.smallFloat = a.smallFloat / b.smallFloat;
        }
        else
        {
            printf("Error: Division by zero\n");
            result.isBig = false;
            result.smallFloat = 0;
        }
    }
    else
    {
        if (!a.isBig)
        {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%lf", a.smallFloat);
            a = createRealFromString(bufA);
        }
        if (!b.isBig)
        {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%lf", b.smallFloat);
            b = createRealFromString(bufB);
        }

        result.isBig = true;
        result.bigFloat = divBigFloat(a.bigFloat, b.bigFloat);
    }

    return result;
}

void freeReal(Real n)
{
    if (n.isBig)
    {
        freeBigFloat(n.bigFloat);
    }
}
