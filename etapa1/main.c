#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
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
        printf("call: ./etapa1 input.txt \n");
        exit(1);
    }
    if (0==(yyin = fopen(argv[1],"r"))) {
        printf("Cannot open file %s... \n",argv[1]);
        exit(1);
    }

    initMe();

    while (isRunning())
    {
        token = yylex();

        if (!isRunning())
            break;

        switch(token)
        {
            case KW_BYTE: fprintf(stdout,"KW_BYTE at line %d \n", getLineNumber()); break;
            case KW_INT: fprintf(stdout,"KW_INT at line %d \n", getLineNumber()); break;
            case KW_FLOAT: fprintf(stdout,"KW_FLOAT at line %d \n", getLineNumber()); break;
            case KW_IF: fprintf(stdout,"KW_IF at line %d \n", getLineNumber()); break;
            case KW_THEN: fprintf(stdout,"KW_THEN at line %d \n", getLineNumber()); break;
            case KW_ELSE: fprintf(stdout,"KW_ELSE at line %d \n", getLineNumber()); break;
            case KW_LOOP: fprintf(stdout,"KW_LOOP at line %d \n", getLineNumber()); break;
            case KW_LEAP: fprintf(stdout,"KW_LEAP at line %d \n", getLineNumber()); break;
            case KW_READ: fprintf(stdout,"KW_READ at line %d \n", getLineNumber()); break;
            case KW_RETURN: fprintf(stdout,"KW_RETURN at line %d \n", getLineNumber()); break;
            case KW_PRINT: fprintf(stdout,"KW_PRINT at line %d \n", getLineNumber()); break;
            case OPERATOR_AND: fprintf(stdout,"OPERATOR_AND at line %d \n", getLineNumber()); break;
            case OPERATOR_OR: fprintf(stdout,"OPERATOR_OR at line %d \n", getLineNumber()); break;
            case OPERATOR_NOT: fprintf(stdout,"OPERATOR_NOT at line %d \n", getLineNumber()); break;
            case OPERATOR_LE: fprintf(stdout,"OPERATOR_LE at line %d \n", getLineNumber()); break;
            case OPERATOR_GE: fprintf(stdout,"OPERATOR_GE at line %d \n", getLineNumber()); break;
            case OPERATOR_EQ: fprintf(stdout,"OPERATOR_EQ at line %d \n", getLineNumber()); break;
            case OPERATOR_DIF: fprintf(stdout,"OPERATOR_DIF at line %d \n", getLineNumber()); break;
            case TK_IDENTIFIER: fprintf(stdout,"TK_IDENTIFIER at line %d \n", getLineNumber()); break;
            case LIT_INTEGER: fprintf(stdout,"LIT_INTEGER at line %d \n", getLineNumber()); break;
            case LIT_FLOAT: fprintf(stdout,"LIT_FLOAT at line %d \n", getLineNumber()); break;
            case LIT_CHAR: fprintf(stdout,"LIT_CHAR at line %d \n", getLineNumber()); break;
            case LIT_STRING: fprintf(stdout,"LIT_STRING at line %d \n", getLineNumber()); break;
            case TOKEN_ERROR: fprintf(stdout,"TOKEN_ERROR at line %d \n", getLineNumber()); break;

            default: fprintf(stdout,"TOKEN <%c> at line %d \n",token,getLineNumber());

        }
    }

    hashPrint();
    exit(0);
  }
