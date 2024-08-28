// lexer.h
#pragma once

#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_INT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

Token getNextToken(const char* input, int* pos);
void freeToken(Token token);

#endif // LEXER_H
