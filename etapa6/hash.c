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
    return hashInsert(SYMBOL_TEMP,buffer);
}

HASH_NODE* makeLabel(void) {
    static int serialNumber = 0;
    static char buffer[64] = "";

    sprintf(buffer,"myLabel%d",serialNumber++);
    return hashInsert(SYMBOL_LABEL,buffer);
}

void hashPrintAsm(FILE *fout) {
    int i,numString = 0;
    HASH_NODE *node;

    for(i=0; i<HASH_SIZE; i++) {
        for(node = Table[i]; node; node=node->next) {
            if((node->type != SYMBOL_FUN) && (node->type != SYMBOL_LABEL)) {

                switch (node->type) {
                case SYMBOL_TEMP:
                    fprintf(fout,"\n_%s:\t.long\t0\n",node->text);
                    break;
                case SYMBOL_LIT_INT:
                    fprintf(fout,"\n_%s:\t.long\t%d\n",node->text,convertInteger(node->text));
                    break;
                case SYMBOL_LIT_STRING:
                    fprintf(fout,"\n_string%d:\t.string\t%s\n",numString,node->text);
                    break;
                default:
                    fprintf(fout,"\n\t.globl	_%s\n"
                                        "\t.data\n"
                                        "\t.type	_%s, @object\n"
                                        "\t.size	_%s, 4\n"
                                        "_%s:\t.long\t%d\n",
                                        node->text,node->text,node->text,node->text,0);
                    break;
                }
            }
        }
    }
}

int convertInteger(char *text) {
    if(strcmp (text, "0") == 0)
        return 0;
    if(strcmp (text, "1") == 0)
        return 1;
    if(strcmp (text, "2") == 0)
        return 2;
    if(strcmp (text, "3") == 0)
        return 3;
    if(strcmp (text, "4") == 0)
        return 4;
    if(strcmp (text, "5") == 0)
        return 5;
    if(strcmp (text, "6") == 0)
        return 6;
    if(strcmp (text, "7") == 0)
        return 7;
    if(strcmp (text, "8") == 0)
        return 8;
    if(strcmp (text, "9") == 0)
        return 9;
    return 0;
}
