// lexer.c
#include "compiler/lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Token createToken(TokenType type, const char* value) {
    Token token;
    token.type = type;
    token.value = strdup(value);
    return token;
}

void freeToken(Token token) {
    free(token.value);
}

Token getNextToken(const char* input, int* pos) {
    while (input[*pos] != '\0') {
        char currentChar = input[*pos];

        if (isspace(currentChar)) {
            (*pos)++;
            continue;
        }

        if (isdigit(currentChar)) {
            int start = *pos;
            while (isdigit(input[*pos])) (*pos)++;
            int length = *pos - start;
            char* value = strndup(input + start, length);
            return createToken(TOKEN_INT, value);
        }

        if (currentChar == '+') {
            (*pos)++;
            return createToken(TOKEN_PLUS, "+");
        }

        if (currentChar == '-') {
            (*pos)++;
            return createToken(TOKEN_MINUS, "-");
        }

        if (currentChar == '*') {
            (*pos)++;
            return createToken(TOKEN_MUL, "*");
        }

        if (currentChar == '/') {
            (*pos)++;
            return createToken(TOKEN_DIV, "/");
        }

        if (currentChar == '(') {
            (*pos)++;
            return createToken(TOKEN_LPAREN, "(");
        }

        if (currentChar == ')') {
            (*pos)++;
            return createToken(TOKEN_RPAREN, ")");
        }

        (*pos)++;
    }

    return createToken(TOKEN_EOF, "");
}
