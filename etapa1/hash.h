#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>

#define HASH_SIZE 997

typedef struct hash_node {
    int type;
    char *text;
    struct hash_node *next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_node *hashInsert(char *text, int type);
void hashPrint(void);

#endif
