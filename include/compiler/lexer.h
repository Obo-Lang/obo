// lexer.h
#pragma once

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Token {
    char* type;
    char* value;
} Token;

#define INITIAL_CAPACITY 100
#define KEYWORDS_COUNT 35

Token* create_token(const char* type, const char* value);
void add_token(Token*** tokens, int* token_count, int* capacity, const char* type, const char* value);
void free_tokens(Token** tokens, int token_count);
int is_keyword(const char* buffer);
Token* get_current_token();
Token* advance();

#endif // LEXER_H
