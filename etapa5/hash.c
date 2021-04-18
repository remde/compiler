#include "hash.h"

void hashInit(void) {
    int i;
    for (i=0; i<HASH_SIZE; ++i) {
        Table[i] = 0;
    }
}

int hashAddress(char *text) {
    int address = 1;
    int i;
    for (i=0; i<strlen(text); ++i) {
        address = (address*text[i]) % HASH_SIZE + 1;
    }
    return address-1;
}

HASH_NODE *hashFind(char *text) {
    HASH_NODE *node;
    int address = hashAddress(text);
    for (node=Table[address]; node; node=node->next) {
        if (strcmp(node->text, text)==0) {
            return node;
        }
    }
    return 0;
}

HASH_NODE *hashInsert(int type, char *text) {
    HASH_NODE *newNode;
    int address = hashAddress(text);

    if ((newNode=hashFind(text))!=0) {
        return newNode;
    }

    newNode = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    newNode->type = type;
    newNode->text = (char*) calloc(strlen(text)+1, sizeof(char));

    switch(type) {
        case SYMBOL_LIT_INT:
            newNode->dataType = SYMBOL_DATATYPE_INT;
            break;
        case SYMBOL_LIT_POINTER:
            newNode->dataType = SYMBOL_DATATYPE_POINTER;
            break;
        case SYMBOL_LIT_CHAR:
            newNode->dataType = SYMBOL_DATATYPE_CHAR;
            break;
        case SYMBOL_LIT_BOOL:
            newNode->dataType = SYMBOL_DATATYPE_BOOL;
            break;
        case SYMBOL_LIT_STRING:
            newNode->dataType = SYMBOL_DATATYPE_STRING;
            break;
    }

    strcpy(newNode->text, text);
    newNode->next = Table[address];
    Table[address] = newNode;
    return newNode;
}

void hashPrint(void) {
    int i;
    HASH_NODE *node;
    for (i=0; i<HASH_SIZE; ++i) {
        for (node=Table[i]; node; node=node->next) {
            printf("Table[%d] has %s\n", i, node->text);
        }
    }
}

void hashCheckUndeclared(void) {
    int i;
    HASH_NODE *node;

    for(i = 0; i < HASH_SIZE; ++i) {
        for(node = Table[i]; node; node=node->next) {
            if(node->type == SYMBOL_IDENTIFIER) {
                fprintf(stderr," Undeclared identifier %s\n", node->text);
            }
        }
    }
}

HASH_NODE* makeTemp(void) {
    static int serialNumber = 0;
    static char buffer[64] = "";

    sprintf(buffer,"myTemp%d",serialNumber++);
    return hashInsert(SYMBOL_IDENTIFIER,buffer);
}

HASH_NODE* makeLabel(void) {
    static int serialNumber = 0;
    static char buffer[64] = "";

    sprintf(buffer,"myLabel%d",serialNumber++);
    return hashInsert(SYMBOL_IDENTIFIER,buffer);
}
