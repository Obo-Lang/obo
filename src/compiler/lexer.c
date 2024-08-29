// lexer.c
#include "compiler/lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Token** tokens;
int token_count = 0;
int current_token_index = 0;

const char* keywords[KEYWORDS_COUNT] = {
    "False", "await", "else", "import", "pass",
    "None", "break", "except", "in", "raise",
    "True", "class", "finally", "is", "return",
    "and", "continue", "for", "lambda", "try",
    "as", "def", "from", "nonlocal", "while",
    "assert", "del", "global", "not", "with",
    "async", "elif", "if", "or", "yield"
};

Token* create_token(const char* type, const char* value) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->type = strdup(type);
    token->value = strdup(value);
    return token;
}

void add_token(Token*** tokens, int* token_count, int* capacity, const char* type, const char* value) {
    if (*token_count >= *capacity) {
        *capacity *= 2;
        *tokens = realloc(*tokens, *capacity * sizeof(Token*));
        if (*tokens == NULL) {
            perror("Ошибка при перераспределении памяти");
            exit(1);
        }
    }
    (*tokens)[*token_count] = create_token(type, value);
    (*token_count)++;
}

void free_tokens(Token** tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]->type);
        free(tokens[i]->value);
        free(tokens[i]);
    }
    free(tokens);
}

int is_keyword(const char* buffer) {
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (strcmp(buffer, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

Token* get_current_token() {
    if (current_token_index < token_count) {
        return tokens[current_token_index];
    }
    return NULL;
}

Token* advance() {
    if (current_token_index < token_count) {
        return tokens[current_token_index++];
    }
    return NULL;
}