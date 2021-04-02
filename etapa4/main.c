#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
#include "y.tab.h"

extern int yylex();
extern char *yytext;
extern FILE *yyin;
FILE *out;
extern int semanticError;

int isRunning(void);
int getLineNumber(void);
void initMe(void);

int main(int argc, char** argv) {
    int token = 0;
    int i = 1;

    if (argc < 3) {
        printf("call: ./etapa4 input.txt output.txt\n");
        exit(1);
    }
    if (0==(yyin = fopen(argv[1],"r"))) {
        printf("Cannot open file %s... \n",argv[1]);
        exit(2);
    }
    if (0==(out = fopen(argv[2],"w"))) {
        printf("Cannot open file %s... \n",argv[2]);
        exit(2);
    }

    initMe();

    yyparse();

    // hashPrint();

    fclose(out);
    fprintf(stderr,"Compiled successfully. \n\n");

    if(semanticError > 0) {
        fprintf(stderr, "Total of %d semantic errors.\n\n", semanticError);
        exit(4);
    }

    exit(0);
  }
