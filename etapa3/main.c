#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
#include "hash.h"

extern int yylex();
extern char *yytext;
extern FILE *yyin;

int isRunning(void);
int getLineNumber(void);
void initMe(void);

int main(int argc, char** argv) {
    int token = 0;
    int i = 1;

    if (argc < 2) {
        printf("call: ./etapa2 input.txt \n");
        exit(1);
    }
    if (0==(yyin = fopen(argv[1],"r"))) {
        printf("Cannot open file %s... \n",argv[1]);
        exit(1);
    }

    initMe();

    yyparse();

    hashPrint();

    printf("Compiled successfully.");

    exit(0);
  }
