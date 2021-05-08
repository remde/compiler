#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>
#include <string.h>

#define HASH_SIZE 997

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_POINTER 2
#define SYMBOL_LIT_BOOL 3
#define SYMBOL_LIT_CHAR 4
#define SYMBOL_LIT_STRING 8
#define SYMBOL_IDENTIFIER 275
#define SYMBOL_LABEL 276
#define SYMBOL_TEMP 277

#define SYMBOL_VAR 5
#define SYMBOL_FUN 6
#define SYMBOL_VET 7

#define SYMBOL_DATATYPE_ERROR 54
#define SYMBOL_DATATYPE_INT 47
#define SYMBOL_DATATYPE_POINTER 48
#define SYMBOL_DATATYPE_BOOL 50
#define SYMBOL_DATATYPE_STRING 51
#define SYMBOL_DATATYPE_CHAR 49

typedef struct hash_node {
    int type;
    int dataType;
    int numParams;
    char *text;
    struct hash_node *next;
} HASH_NODE;

HASH_NODE*Table[HASH_SIZE];

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(int type, char *text);
void hashPrint(void);
void hashCheckUndeclared(void);

HASH_NODE* makeTemp(void);
HASH_NODE* makeLabel(void);

void hashPrintAsm(FILE *fout);

#endif
