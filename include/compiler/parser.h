// parser.h
#pragma once

#ifndef PARSER_H
#define PARSER_H

#include "compiler/lexer.h"

typedef struct ASTNode {
    Token token;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

ASTNode* expr(const char* input, int* pos);
void freeAST(ASTNode* node);

#endif // PARSER_H
