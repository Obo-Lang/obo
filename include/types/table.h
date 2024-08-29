// table.h
#pragma once

#include <stdio.h>

#ifndef TABLE_H
#define TABLE_H

typedef struct Node
{
    int value;
    struct Node *next;
} Node;

void push(Node **head, int data);
int pop(Node **head);
Node *getNth(Node *head, int n);
Node *getLast(Node *head);
void pushBack(Node *head, int value);
Node *getLastButOne(Node *head);
int popBack(Node **head);
void insert(Node *head, unsigned n, int val);
int deleteNth(Node **head, int n);
void deleteList(Node **head);
int len(const Node *head);
void fromArray(Node **head, int *arr, size_t size);
int *toArray(const Node *head);
void printLinkedList(const Node *head);

#endif // CODEGEN_H