#include "compiler/codegen.h"
#include <stdio.h>

void generateCode(ASTNode* node) {
    if (node == NULL) return;

    generateCode(node->left);
    generateCode(node->right);

    switch (node->token.type) {
        case TOKEN_INT:
            printf("PUSH %s\n", node->token.value);
            break;
        case TOKEN_PLUS:
            printf("ADD\n");
            break;
        case TOKEN_MINUS:
            printf("SUB\n");
            break;
        case TOKEN_MUL:
            printf("MUL\n");
            break;
        case TOKEN_DIV:
            printf("DIV\n");
            break;
        default:
            break;
    }
}
