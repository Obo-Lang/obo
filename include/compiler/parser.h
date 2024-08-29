// parser.h
#pragma once

#ifndef PARSER_H
#define PARSER_H

typedef struct ASTNode {
    char* type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

ASTNode* create_ast_node(const char* type, const char* value);
ASTNode* parse_factor();
ASTNode* parse_term();
ASTNode* parse_expr();
ASTNode* parse_assignment();
void print_ast(ASTNode* node, int level);
void free_ast(ASTNode* node);

#endif // PARSER_H
