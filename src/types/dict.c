// dict.c
#include "types/dict.h"

#include <stdlib.h>
#include <string.h>

unsigned int hash(const char *key)
{
    unsigned int hash = 0;
    while (*key)
    {
        hash = (hash << 5) + *key++;
    }
    return hash % TABLE_SIZE;
}

HashTable *createDict()
{
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));
    if (!newTable)
    {
        perror("Failed to create hash table");
        return NULL;
    }
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        newTable->table[i] = NULL;
    }
    return newTable;
}

void insertDict(HashTable *table, const char *key, Value value)
{
    unsigned int index = hash(key);
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        perror("Failed to insert into hash table");
        return;
    }
    strncpy(
        newNode->key,
        key,
        sizeof(newNode->key));
    newNode->value = value;
    newNode->next = table->table[index];
    table->table[index] = newNode;
}

Value *searchDict(HashTable *table, const char *key)
{
    unsigned int index = hash(key);
    Node *current = table->table[index];
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            return &current->value;
        }
        current = current->next;
    }
    return NULL;
}

void freeDict(HashTable *table)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Node *current = table->table[i];
        while (current)
        {
            Node *toDelete = current;
            current = current->next;

            if (toDelete->value.type == STRING)
            {
                free(toDelete->value.data.s);
            }

            free(toDelete);
        }
    }
    free(table);
}
