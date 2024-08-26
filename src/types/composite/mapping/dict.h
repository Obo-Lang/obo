// dict.h

#ifndef DICT_H
#define DICT_H

#include <stdio.h>
#include "includes.h"

#define TABLE_SIZE 100

typedef enum { INT, DOUBLE, STRING, POINTER } ValueType;

typedef struct Value {
    ValueType type;
    union {
        int i;
        double d;
        char* s;
        void* p;
    } data;
} Value;

typedef struct Node {
    char key[50];
    Value value;
    struct Node* next;
} Node;

typedef struct HashTable {
    Node* table[TABLE_SIZE];
} HashTable;

HashTable* createTable();

void insert(
    HashTable* table, 
    const char* key, 
    Value value
);

Value* search(
    HashTable* table, 
    const char* key
);

void freeTable(HashTable* table);

#endif
