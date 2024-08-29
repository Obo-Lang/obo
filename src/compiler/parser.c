// parser.c
#include "compiler/parser.h"
#include "compiler/lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* create_ast_node(const char* type, const char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = strdup(type);
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* parse_factor() {
    Token* token = get_current_token();
    if (token == NULL) return NULL;

    if (strcmp(token->type, "NUMBER") == 0 || strcmp(token->type, "IDENTIFIER") == 0) {
        advance();
        return create_ast_node(token->type, token->value);
    } else if (strcmp(token->type, "SYMBOL") == 0 && strcmp(token->value, "(") == 0) {
        advance();
        ASTNode* node = parse_expr();
        token = get_current_token();
        if (token == NULL || strcmp(token->type, "SYMBOL") != 0 || strcmp(token->value, ")") != 0) {
            printf("Ошибка синтаксиса: ожидается ')'\n");
            exit(1);
        }
        advance();
        return node;
    }

    printf("Ошибка синтаксиса: ожидается фактор\n");
    exit(1);
    return NULL;
}

ASTNode* parse_term() {
    ASTNode* node = parse_factor();
    Token* token = get_current_token();

    while (token && (strcmp(token->value, "*") == 0 || strcmp(token->value, "/") == 0)) {
        advance();
        ASTNode* new_node = create_ast_node("BINARY_OP", token->value);
        new_node->left = node;
        new_node->right = parse_factor();
        node = new_node;
        token = get_current_token();
    }

    return node;
}

ASTNode* parse_expr() {
    ASTNode* node = parse_term();
    Token* token = get_current_token();

    while (token && (strcmp(token->value, "+") == 0 || strcmp(token->value, "-") == 0)) {
        advance();
        ASTNode* new_node = create_ast_node("BINARY_OP", token->value);
        new_node->left = node;
        new_node->right = parse_term();
        node = new_node;
        token = get_current_token();
    }

    return node;
}

ASTNode* parse_assignment() {
    Token* token = get_current_token();
    if (token == NULL || strcmp(token->type, "IDENTIFIER") != 0) {
        printf("Ошибка синтаксиса: ожидается идентификатор\n");
        exit(1);
    }

    ASTNode* node = create_ast_node("ASSIGNMENT", token->value);
    advance();

    token = get_current_token();
    if (token == NULL || strcmp(token->value, "=") != 0) {
        printf("Ошибка синтаксиса: ожидается '='\n");
        exit(1);
    }
    advance();

    node->right = parse_expr();
    return node;
}

void print_ast(ASTNode* node, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("{ type: %s, value: %s }\n", node->type, node->value);

    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
}

void free_ast(ASTNode* node) {
    if (node) {
        free(node->type);
        free(node->value);
        free_ast(node->left);
        free_ast(node->right);
        free(node);
    }
}
