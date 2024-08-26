// number.c

#include "number.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

BigInt createBigInt(const char *str) {
    BigInt result;
    result.length = strlen(str);
    result.isNegative = (str[0] == '-');
    result.digits = (char *)malloc(result.length + 1);
    strcpy(result.digits, str + result.isNegative);
    
    return result;
}

void freeBigInt(BigInt n) {
    free(n.digits);
}

Number createNumberFromString(const char *str) {
    Number result;

    char *endptr;
    int value = strtol(str, &endptr, 10);

    if (*endptr == '\0' && (value != LONG_MAX && value != LONG_MIN)) {
        result.isBig = false;
        result.smallInt = value;
    } else {
        result.isBig = true;
        result.bigInt = createBigInt(str);
    }

    return result;
}

Number createNumberFromInt(int value) {
    Number result;
    result.isBig = false;
    result.smallInt = value;
    return result;
}

void printNumber(Number n) {
    if (n.isBig) {
        if (n.bigInt.isNegative) {
            printf("-");
        }
        printf("%s\n", n.bigInt.digits);
    } else {
        printf("%d\n", n.smallInt);
    }
}

BigInt addBigInt(BigInt a, BigInt b) {
    int maxLength = (a.length > b.length) ? a.length : b.length;
    char *result = (char *)malloc(maxLength + 2);
    
    int carry = 0;
    int i;
    
    for (i = 0; i < maxLength; i++) {
        int digitA = (i < a.length) ? a.digits[a.length - 1 - i] - '0' : 0;
        int digitB = (i < b.length) ? b.digits[b.length - 1 - i] - '0' : 0;
        int sum = digitA + digitB + carry;
        result[maxLength - i] = (sum % 10) + '0';
        carry = sum / 10;
    }
    
    if (carry) {
        result[0] = carry + '0';
        result[maxLength + 1] = '\0';
    } else {
        strcpy(result, result + 1);
    }
    
    BigInt sumResult;
    sumResult.length = strlen(result);
    sumResult.digits = result;
    sumResult.isNegative = false;
    
    return sumResult;
}

BigInt subBigInt(BigInt a, BigInt b) {
    int maxLength = (a.length > b.length) ? a.length : b.length;
    char *result = (char *)malloc(maxLength + 1);
    
    int borrow = 0;
    int i;
    
    for (i = 0; i < maxLength; i++) {
        int digitA = (i < a.length) ? a.digits[a.length - 1 - i] - '0' : 0;
        int digitB = (i < b.length) ? b.digits[b.length - 1 - i] - '0' : 0;
        int diff = digitA - digitB - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result[maxLength - i - 1] = diff + '0';
    }
    
    int start = 0;
    while (start < maxLength - 1 && result[start] == '0') {
        start++;
    }
    
    BigInt subResult;
    subResult.length = maxLength - start;
    subResult.digits = (char *)malloc(subResult.length + 1);
    strncpy(subResult.digits, result + start, subResult.length);
    subResult.digits[subResult.length] = '\0';
    subResult.isNegative = false;
    
    free(result);
    
    return subResult;
}

BigInt mulBigInt(BigInt a, BigInt b) {
    int maxLength = a.length + b.length;
    char *result = (char *)calloc(maxLength + 1, sizeof(char));
    
    for (int i = 0; i < a.length; i++) {
        int carry = 0;
        for (int j = 0; j < b.length; j++) {
            int product = (a.digits[a.length - 1 - i] - '0') * (b.digits[b.length - 1 - j] - '0') + carry + (result[maxLength - 1 - i - j] - '0');
            result[maxLength - 1 - i - j] = (product % 10) + '0';
            carry = product / 10;
        }
        result[maxLength - 1 - i - b.length] += carry;
    }
    
    int start = 0;
    while (start < maxLength - 1 && result[start] == '0') {
        start++;
    }
    
    BigInt mulResult;
    mulResult.length = maxLength - start;
    mulResult.digits = (char *)malloc(mulResult.length + 1);
    strncpy(mulResult.digits, result + start, mulResult.length);
    mulResult.digits[mulResult.length] = '\0';
    mulResult.isNegative = false;
    
    free(result);
    
    return mulResult;
}

BigInt divBigInt(BigInt a, BigInt b) {
    // TODO
    BigInt result;
    result.length = 0;
    result.digits = NULL;
    result.isNegative = false;
    
    return result;
}

Number addNumber(Number a, Number b) {
    Number result;
    
    if (!a.isBig && !b.isBig) {
        result.isBig = false;
        result.smallInt = a.smallInt + b.smallInt;
    } else {
        if (!a.isBig) {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%d", a.smallInt);
            a = createNumberFromString(bufA);
        }
        if (!b.isBig) {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%d", b.smallInt);
            b = createNumberFromString(bufB);
        }
        
        result.isBig = true;
        result.bigInt = addBigInt(a.bigInt, b.bigInt);
    }
    
    return result;
}

Number subNumber(Number a, Number b) {
    Number result;
    
    if (!a.isBig && !b.isBig) {
        result.isBig = false;
        result.smallInt = a.smallInt - b.smallInt;
    } else {
        if (!a.isBig) {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%d", a.smallInt);
            a = createNumberFromString(bufA);
        }
        if (!b.isBig) {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%d", b.smallInt);
            b = createNumberFromString(bufB);
        }
        
        result.isBig = true;
        result.bigInt = subBigInt(a.bigInt, b.bigInt);
    }
    
    return result;
}

Number mulNumber(Number a, Number b) {
    Number result;
    
    if (!a.isBig && !b.isBig) {
        result.isBig = false;
        result.smallInt = a.smallInt * b.smallInt;
    } else {
        if (!a.isBig) {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%d", a.smallInt);
            a = createNumberFromString(bufA);
        }
        if (!b.isBig) {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%d", b.smallInt);
            b = createNumberFromString(bufB);
        }
        
        result.isBig = true;
        result.bigInt = mulBigInt(a.bigInt, b.bigInt);
    }
    
    return result;
}

Number divNumber(Number a, Number b) {
    Number result;
    
    if (!a.isBig && !b.isBig) {
        if (b.smallInt != 0) {
            result.isBig = false;
            result.smallInt = a.smallInt / b.smallInt;
        } else {
            printf("Error: Division by zero\n");
            result.isBig = false;
            result.smallInt = 0;
        }
    } else {
        if (!a.isBig) {
            char bufA[50];
            snprintf(bufA, sizeof(bufA), "%d", a.smallInt);
            a = createNumberFromString(bufA);
        }
        if (!b.isBig) {
            char bufB[50];
            snprintf(bufB, sizeof(bufB), "%d", b.smallInt);
            b = createNumberFromString(bufB);
        }
        
        result.isBig = true;
        result.bigInt = divBigInt(a.bigInt, b.bigInt);
    }
    
    return result;
}

void freeNumber(Number n) {
    if (n.isBig) {
        freeBigInt(n.bigInt);
    }
}
