#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern FILE *out;

AST *astCreate(int type, HASH_NODE *symbol, AST *c0, AST *c1, AST *c2, AST *c3) {
    AST *newNode;

    newNode = (AST*)calloc(1,sizeof(AST));

    newNode->type = type;
    newNode->symbol = symbol;

    newNode->child[0] = c0;
    newNode->child[1] = c1;
    newNode->child[2] = c2;
    newNode->child[3] = c3;

    return newNode;
}

void astPrint(int level, AST *node) {
    int i;

    if(!node) return;

    for(i=0;i<level;i++)
	    fprintf(stderr," ");
    
    fprintf(stderr,"AST(");

    switch (node->type)
    {
        case AST_SYMBOL:
            fprintf(stderr,"AST_SYMBOL,");
            break;
        case AST_ADD:
            fprintf(stderr,"AST_ADD,");
            break;
        case AST_SUB:
            fprintf(stderr,"AST_SUB,");
            break;
        case AST_MULT:
            fprintf(stderr,"AST_MULT,");
            break;
        case AST_DIV:
            fprintf(stderr,"AST_DIV,");
            break;
        case AST_LESS:
            fprintf(stderr,"AST_LESS,");
            break;
        case AST_GREATER:
            fprintf(stderr,"AST_GREATER,");
            break;
        case AST_LE:
            fprintf(stderr,"AST_LE,");
            break;
        case AST_GE:
            fprintf(stderr,"AST_GE,");
            break;
        case AST_EQ:
            fprintf(stderr,"AST_EQ,");
            break;
        case AST_DIF:
            fprintf(stderr,"AST_DIF,");
            break;
        case AST_OR:
            fprintf(stderr,"AST_OR,");
            break;
        case AST_AND:
            fprintf(stderr,"AST_AND,");
            break;
        case AST_NOT:
            fprintf(stderr,"AST_NOT,");
            break;
        case AST_IFT:
            fprintf(stderr,"AST_IFT,");
            break;
        case AST_IFTE:
            fprintf(stderr,"AST_IFTE,");
            break;
        case AST_WHILE:
            fprintf(stderr,"AST_WHILE,");
            break;
        case AST_PRINT:
            fprintf(stderr,"AST_PRINT,");
            break;
        case AST_READ:
            fprintf(stderr,"AST_READ,");
            break;
        case AST_RETURN:
            fprintf(stderr,"AST_RETURN,");
            break;
        case AST_LCMD:
            fprintf(stderr,"AST_LCMD,");
            break;
        case AST_DECFUNC:
            fprintf(stderr,"AST_DECFUNC,");
            break;
        case AST_VARDEC:
            fprintf(stderr,"AST_VARDEC,");
            break;
        case AST_ASSIGN:
            fprintf(stderr,"AST_ASSIGN,");
            break;
        case AST_ASSIGNARRAY:
            fprintf(stderr,"ASSIGNARRAY,");
            break;
        case AST_ARRAY:
            fprintf(stderr,"AST_ARRAY,");
            break;
        case AST_BLOCK:
            fprintf(stderr,"AST_BLOCK,");
            break;
        case AST_PARENTH:
            fprintf(stderr,"AST_PARENTH,");
            break;
        case AST_DECFUNC_VOID:
            fprintf(stderr,"AST_DECFUNC_VOID,");
            break;
        case AST_VETDEC:
            fprintf(stderr,"AST_VETDEC,");
            break;
        case AST_DEC:
            fprintf(stderr,"AST_DEC,");
            break;
        case AST_DEC_PARAM:
            fprintf(stderr,"AST_DEC_PARAM,");
            break;
        case AST_DEC_PARAM2:
            fprintf(stderr,"AST_DEC_PARAM2,");
            break;
        case AST_PRINT_PARAM:
            fprintf(stderr,"AST_PRINT_PARAM,");
            break;
        case AST_INT:
            fprintf(stderr,"AST_INT,");
            break;
        case AST_BOOL:
            fprintf(stderr,"AST_BOOL,");
            break;
        case AST_POINTER:
            fprintf(stderr,"AST_POINTER,");
            break;
        case AST_CHAR:
            fprintf(stderr,"AST_CHAR,");
            break;
        case AST_ARRAY_INIT:
            fprintf(stderr,"AST_ARRAY_INIT,");
            break;
        case AST_ARRAY_INIT2:
            fprintf(stderr,"AST_ARRAY_INIT2,");
            break;
        case AST_FUNCCALL:
            fprintf(stderr,"AST_FUNCCALL,");
            break;
        case AST_FUNC_PARAM:
            fprintf(stderr,"AST_FUNC_PARAM,");
            break;
        case AST_FUNC_PARAM2:
            fprintf(stderr,"AST_FUNC_PARAM2,");
            break;
        case AST_VECSIZE:
            fprintf(stderr,"AST_VECSIZE,");
            break;
        case AST_DOLLAR:
            fprintf(stderr,"AST_DOLLAR,");
            break;
        case AST_HASH:
            fprintf(stderr,"AST_HASH");
        default:
            fprintf(stderr,"AST_UNKNOWN,");
        break;
    }
    
    if(node->symbol)
        fprintf(stderr,"%s,\n", node->symbol->text);
    else
        fprintf(stderr,"'',\n");
    
    for(i = 0; i < MAX_CHILDREN; ++i)
        astPrint(level+1, node->child[i]);
}

void astDecompile(AST *node) {
    int i;

    if(!node) return;
    
    switch (node->type) {
        case AST_SYMBOL:
            fprintf(out,"%s", node->symbol->text);
            break;
        case AST_ADD:
            astDecompile(node->child[0]); 
            fprintf(out," + "); 
            astDecompile(node->child[1]);
            break;
        case AST_SUB:
            astDecompile(node->child[0]); 
            fprintf(out," - "); 
            astDecompile(node->child[1]);
            break;
        case AST_MULT:
            astDecompile(node->child[0]); 
            fprintf(out," * "); 
            astDecompile(node->child[1]);
            break;
        case AST_DIV:
            astDecompile(node->child[0]); 
            fprintf(out," / "); 
            astDecompile(node->child[1]);
            break;
        case AST_LESS:
            astDecompile(node->child[0]); 
            fprintf(out," < "); 
            astDecompile(node->child[1]);
            break;
        case AST_GREATER:
            astDecompile(node->child[0]); 
            fprintf(out," > "); 
            astDecompile(node->child[1]);
            break;
        case AST_LE:
            astDecompile(node->child[0]); 
            fprintf(out," <= "); 
            astDecompile(node->child[1]);
            break;
        case AST_GE:
            astDecompile(node->child[0]); 
            fprintf(out," >= "); 
            astDecompile(node->child[1]);
            break;
        case AST_EQ:
            astDecompile(node->child[0]); 
            fprintf(out," == "); 
            astDecompile(node->child[1]);
            break;
        case AST_DIF:
            astDecompile(node->child[0]); 
            fprintf(out," != "); 
            astDecompile(node->child[1]);
            break;
        case AST_OR:
            astDecompile(node->child[0]); 
            fprintf(out," | "); 
            astDecompile(node->child[1]);
            break;
        case AST_AND:
            astDecompile(node->child[0]); 
            fprintf(out," & "); 
            astDecompile(node->child[1]);
            break;
        case AST_NOT:
            fprintf(out,"~"); 
            astDecompile(node->child[0]);
            break;
        case AST_IFT:
            fprintf(out,"if("); 
            astDecompile(node->child[0]); 
            fprintf(out,") then \n"); 
            astDecompile(node->child[1]); 
            fprintf(out," ");
            break;
        case AST_IFTE:
            fprintf(out,"if("); 
            astDecompile(node->child[0]); 
            fprintf(out,") then "); 
            astDecompile(node->child[1]); 
            fprintf(out," "); 
            fprintf(out,"else "); 
            astDecompile(node->child[2]);
            break;
        case AST_WHILE:
            fprintf(out,"while("); 
            astDecompile(node->child[0]); 
            fprintf(out,")"); 
            astDecompile(node->child[1]);
            break;
        case AST_PRINT:
            fprintf(out,"print "); 
            astDecompile(node->child[0]);
            break;
        case AST_READ:
            fprintf(out,"read "); 
            fprintf(out,"%s", node->symbol->text);
            break;
        case AST_RETURN:
            fprintf(out,"return "); 
            astDecompile(node->child[0]);
            break;
        case AST_LCMD:
            astDecompile(node->child[0]); 
            fprintf(out,";\n");
            if(node->child[1] != NULL) {
                astDecompile(node->child[1]);
            }
            break;  
        case AST_DECFUNC:
            astDecompile(node->child[0]); 
            fprintf(out," "); 
            fprintf(out,"%s", node->symbol->text); 
            fprintf(out,"("); 
            astDecompile(node->child[1]); 
            fprintf(out,")"); 
            astDecompile(node->child[2]);
            break;
        case AST_VARDEC:
            astDecompile(node->child[0]);  
            fprintf(out," "); 
            fprintf(out,"%s", node->symbol->text); 
            fprintf(out,":"); 
            astDecompile(node->child[1]); 
            fprintf(out,";"); 
            astDecompile(node->child[2]);
            break;
        case AST_ASSIGN:
            fprintf(out,"%s", node->symbol->text); 
            fprintf(out,":"); 
            astDecompile(node->child[0]);
            break;
        case AST_ASSIGNARRAY:
            fprintf(out, "%s", node->symbol->text); 
            fprintf(out, "["); 
            astDecompile(node->child[0]);
            fprintf(out, "]"); 
            fprintf(out, "<-"); 
            astDecompile(node->child[1]);
            break;
        case AST_ARRAY:
            fprintf(out,"%s", node->symbol->text); 
            fprintf(out,"["); 	
            astDecompile(node->child[0]); 
            fprintf(out,"]"); 	
            astDecompile(node->child[1]); 
			if(node->child[2]!=NULL) {
				astDecompile(node->child[2]);
            }
            break;
        case AST_BLOCK:
            fprintf(out,"\n{\n"); 
            astDecompile(node->child[0]); 
            fprintf(out,"}");
            break;
        case AST_PARENTH:
            fprintf(out,"("); 
            astDecompile(node->child[0]); 
            fprintf(out,")");
            break;
        case AST_DECFUNC_VOID:
            astDecompile(node->child[0]); 
            fprintf(out," %s", node->symbol->text); 
            fprintf(out, "()"); 
            astDecompile(node->child[1]);
            break;
        case AST_VETDEC:
            astDecompile(node->child[0]); 
            fprintf(out,"["); 
            astDecompile(node->child[1]); 
            fprintf(out,"]");
            fprintf(out," %s", node->symbol->text); 
			if(node->child[2]!=NULL) {
                fprintf(out,": "); astDecompile(node->child[2]);
            }
			fprintf(out,";");
            break;
        case AST_DEC:
            if(node->child[1] != NULL) {
                astDecompile(node->child[0]); fprintf(out," \n"); astDecompile(node->child[1]);
            }	
            else
                astDecompile(node->child[0]);
            break;
        case AST_DEC_PARAM:
            astDecompile(node->child[0]); 
            fprintf(out," "); 
            fprintf(out,"%s", node->symbol->text); 
            astDecompile(node->child[1]);
            break;
        case AST_DEC_PARAM2:
           fprintf(out,", ");
		    astDecompile(node->child[0]);
            break;
        case AST_PRINT_PARAM:
            if(node->child[1] != NULL) {
        		astDecompile(node->child[0]); 
                fprintf(out,","); 
                astDecompile(node->child[1]);
            }
            else
           	 	astDecompile(node->child[0]);
            break;
        case AST_INT:
            fprintf(out, "int");
            break;
        case AST_BOOL:
            fprintf(out, "bool");
            break;
        case AST_POINTER:
            fprintf(out, "pointer");
            break;
        case AST_CHAR:
            fprintf(out, "char");
            break;
        case AST_ARRAY_INIT:
            astDecompile(node->child[0]);
            if(node->child[1] != NULL) {
                fprintf(out," "); 
                astDecompile(node->child[1]);
        	}
            break;
        case AST_ARRAY_INIT2:
            astDecompile(node->child[0]);
            if(node->child[1] != NULL) {
                fprintf(out," ");
                astDecompile(node->child[1]);
        	}
            break;
        case AST_FUNCCALL:
            fprintf(out,"%s", node->symbol->text); 
            fprintf(out,"("); 
            astDecompile(node->child[0]); 
            fprintf(out,")");
            break;
        case AST_FUNC_PARAM:
            astDecompile(node->child[0]);
            if(node->child[1] != NULL) {
            	fprintf(out,","); 
                astDecompile(node->child[1]);
        	}
            break;
        case AST_FUNC_PARAM2:
            astDecompile(node->child[0]);
            if(node->child[1] != NULL) {
            	fprintf(out,","); 
                astDecompile(node->child[1]);
        	}
            break;
        case AST_VECSIZE:
            fprintf(out,"%s", node->symbol->text);
            break;
        case AST_DOLLAR:
            fprintf(out,"$ "); 
            astDecompile(node->child[0]);
            break;
        case AST_HASH:
            fprintf(out,"# "); 
            astDecompile(node->child[0]);
            break;


        default:
            fprintf(out,"\nUNKNOWN\n");
        break;
    }

    return;
}
