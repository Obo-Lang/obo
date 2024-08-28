// parser.c
#include "compiler/parser.h"

#include <stdlib.h>
#include <string.h> 

char* strndup(const char* str, size_t n) {
    char* dup_str = (char*)malloc(n + 1);
    if (dup_str) {
        strncpy(dup_str, str, n);
        dup_str[n] = '\0';
    }
    return dup_str;
}

ASTNode* createASTNode(Token token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeAST(ASTNode* node) {
    if (node) {
        freeToken(node->token);
        freeAST(node->left);
        freeAST(node->right);
        free(node);
    }
}

void eat(TokenType type, Token* currentToken, const char* input, int* pos) {
    if (currentToken->type == type) {
        *currentToken = getNextToken(input, pos);
    } else {
        // Handle error
    }
}

ASTNode* factor(const char* input, int* pos, Token* currentToken) {
    if (currentToken->type == TOKEN_INT) {
        ASTNode* node = createASTNode(*currentToken);
        eat(TOKEN_INT, currentToken, input, pos);
        return node;
    } else if (currentToken->type == TOKEN_LPAREN) {
        eat(TOKEN_LPAREN, currentToken, input, pos);
        ASTNode* node = expr(input, pos);
        eat(TOKEN_RPAREN, currentToken, input, pos);
        return node;
    }

    return NULL;
}

ASTNode* term(const char* input, int* pos, Token* currentToken) {
    ASTNode* node = factor(input, pos, currentToken);

    while (currentToken->type == TOKEN_MUL || currentToken->type == TOKEN_DIV) {
        Token token = *currentToken;
        if (token.type == TOKEN_MUL) {
            eat(TOKEN_MUL, currentToken, input, pos);
        } else if (token.type == TOKEN_DIV) {
            eat(TOKEN_DIV, currentToken, input, pos);
        }
        ASTNode* newNode = createASTNode(token);
        newNode->left = node;
        newNode->right = factor(input, pos, currentToken);
        node = newNode;
    }

    return node;
}

ASTNode* expr(const char* input, int* pos) {
    Token currentToken = getNextToken(input, pos);
    ASTNode* node = term(input, pos, &currentToken);

    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        Token token = currentToken;
        if (token.type == TOKEN_PLUS) {
            eat(TOKEN_PLUS, &currentToken, input, pos);
        } else if (token.type == TOKEN_MINUS) {
            eat(TOKEN_MINUS, &currentToken, input, pos);
        }
        ASTNode* newNode = createASTNode(token);
        newNode->left = node;
        newNode->right = term(input, pos, &currentToken);
        node = newNode;
    }

    return node;
}
