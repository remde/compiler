#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"

int semanticError = 0;
AST *nodeDeclared;

void setTypes(AST* node);
int numParamsFunc(AST* node);
int checkVet(AST* node, int dataType);
int getType(AST* node);
int expressionTypes(int op1,int op2);
int functionValidation (AST* nodeDeclared, AST *node);
int validReturn(AST *nodeDec,AST *node);
int checkParams(AST* node);

void setAndCheckRedeclared(AST *node) {
    int i;

    if(nodeDeclared==NULL)
		nodeDeclared=node;

	if (node == 0)
        return;

	for (i = 0; i < MAX_CHILDREN; ++i)
		setAndCheckRedeclared(node->child[i]);

	switch(node->type) {
		case AST_VARDEC:
			if(node->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s redeclared type=%d \n", node->lineNumber,node->symbol->text,node->symbol->type);
                semanticError++;
            }
			else {
				node->symbol->type = SYMBOL_VAR;
				setTypes(node);
			}
		break;
		case AST_VETDEC:
			if(node->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s redeclared type=%d \n", node->lineNumber,node->symbol->text,node->symbol->type);
                semanticError++;
            }
			else {
				node->symbol->type = SYMBOL_VET;
				setTypes(node);
			}
			break;
		case AST_DECFUNC_VOID:
			if(node->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s redeclared type=%d \n", node->lineNumber,node->symbol->text,node->symbol->type);
                semanticError++;
            }
			else {
				node->symbol->type = SYMBOL_FUN;
				node->symbol->numParams = 0;
				setTypes(node);
			}
		break;
        case AST_DECFUNC:
			if(node->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s redeclared type=%d \n", node->lineNumber,node->symbol->text,node->symbol->type);
                semanticError++;
            }
			else {
				node->symbol->type = SYMBOL_FUN;
				node->symbol->numParams = numParamsFunc(node->child[1]);
				setTypes(node);
			}
		break;
		case AST_DEC_PARAM:
			if(node->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s redeclared type=%d \n", node->lineNumber,node->symbol->text,node->symbol->type);
                semanticError++;
            }
			else {
				node->symbol->type = SYMBOL_VAR;
				setTypes(node);
			}
		break;
	}
}

void checkOperands(AST *node) {
    int i;

	if(node == 0)
        return;

	for(i = 0; i < MAX_CHILDREN; ++i)
		checkOperands(node->child[i]);

	switch(node->type) {
        case AST_SYMBOL:
            switch (node->symbol->type) {
                case SYMBOL_VAR:
                    if(node->symbol->dataType == SYMBOL_DATATYPE_INT) {
                        node->dataType = AST_DATATYPE_INT;
                    }
                    else if(node->symbol->dataType == SYMBOL_DATATYPE_CHAR) {
                        node->dataType = AST_DATATYPE_CHAR;
                    }
                    else if(node->symbol->dataType == SYMBOL_DATATYPE_BOOL) {
                        node->dataType = AST_DATATYPE_BOOL;
                    }
                    else if(node->symbol->dataType == SYMBOL_DATATYPE_POINTER) {
                        node->dataType = AST_DATATYPE_POINTER;
                    }
                    break;
                case SYMBOL_VET:
				    node->dataType = AST_DATATYPE_VET;
                    break;
                case SYMBOL_FUN:
				    node->dataType = AST_DATATYPE_FUN;
                    break;
                case SYMBOL_LIT_INT:
				    node->dataType = AST_DATATYPE_INT;
                    break;
                case SYMBOL_LIT_POINTER:
				    node->dataType = AST_DATATYPE_POINTER;
                    break;
                case SYMBOL_LIT_BOOL:
				    node->dataType = AST_DATATYPE_BOOL;
                    break;
                case SYMBOL_LIT_CHAR:
				    node->dataType = AST_DATATYPE_CHAR;
                    break;
                case SYMBOL_LIT_STRING:
				    node->dataType = AST_DATATYPE_STRING;
                    break;
                default:
                    break;
            }
        break;
		case AST_VARDEC:
			if(node->child[1]->symbol != NULL) {
                if((node->symbol->dataType == SYMBOL_DATATYPE_INT || node->symbol->dataType == SYMBOL_DATATYPE_CHAR) &&
                    (node->child[1]->symbol->dataType != SYMBOL_DATATYPE_INT && node->child[1]->symbol->dataType != SYMBOL_DATATYPE_CHAR)) {
                    fprintf(stderr,"SEMANTIC ERROR in line %d. Variable %s must receive int or char, not %d.\n", node->lineNumber,node->symbol->text, node->child[1]->symbol->dataType);
                    semanticError++;
				}
				else if(node->symbol->dataType == SYMBOL_DATATYPE_BOOL && node->child[1]->symbol->dataType != SYMBOL_DATATYPE_BOOL) {
                    fprintf(stderr,"SEMANTIC ERROR in line %d. Variable %s must receive bool.\n", node->lineNumber,node->symbol->text);
                    semanticError++;
				}
                /* else if(node->symbol->dataType == SYMBOL_DATATYPE_POINTER && node->child[1]->symbol->dataType != SYMBOL_DATATYPE_POINTER) { */
                /*     fprintf(stderr,"SEMANTIC ERROR in line %d. Variable %s must receive valid pointer.\n", node->lineNumber,node->symbol->text); */
                /*     semanticError++; */
                /* } */
			}
            break;
        case AST_VETDEC:
			if(checkVet(node->child[2], node->symbol->dataType) == 0) {
                fprintf(stderr,"SEMANTIC ERROR in line %d. Invalid types in vector declaration\n", node->lineNumber);
                semanticError++;
            }
            break;
		case AST_DECFUNC_VOID:
        case AST_DECFUNC:
            if(validReturn(node,node)==0) {
                    fprintf(stderr, "SEMANTIC ERROR in line %d. Invalid return type in function %s.\n",node->lineNumber,node->symbol->text);
                    semanticError++;
            }
            break;
		case AST_ASSIGN:
			if(node->symbol->type != SYMBOL_VAR) {
                fprintf(stderr,"SEMANTIC ERROR in line %d. Symbol %s must be scalar.\n",node->lineNumber, node->symbol->text);
                semanticError++;
            }
            if(node->symbol->dataType == SYMBOL_DATATYPE_INT || node->symbol->dataType == SYMBOL_DATATYPE_CHAR) {
				if(node->child[0] != NULL) {
					if(node->child[0]->dataType != AST_DATATYPE_INT && node->child[0]->dataType != AST_DATATYPE_CHAR) {
                        fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s must be assigned to char or int.\n",node->lineNumber,node->symbol->text);
						semanticError++;
					}
				}
            }
			else if(node->symbol->dataType == SYMBOL_DATATYPE_POINTER) {
				if(node->child[0] != NULL) {
					if(node->child[0]->dataType != AST_DATATYPE_POINTER) {
                        fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s must be assigned to pointer.\n",node->lineNumber,node->symbol->text);
						semanticError++;
					}
				}
			}
			break;
		case AST_ASSIGNARRAY:
			if(node->symbol->type != SYMBOL_VET) {
				fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s is not a vector.\n",node->lineNumber,node->symbol->text);
                semanticError++;
			}

			if(node->child[0]->dataType != AST_DATATYPE_INT && node->child[0]->dataType != AST_DATATYPE_CHAR) {
				fprintf(stderr,"SEMANTIC ERROR in line %d. Invalid index type in assignment, must be char or int.\n",node->lineNumber);
                semanticError++;
			}

            if(node->symbol->dataType == SYMBOL_DATATYPE_INT || node->symbol->dataType == SYMBOL_DATATYPE_CHAR) {
				if(node->child[1] != NULL) {
					if(node->child[1]->dataType != AST_DATATYPE_INT && node->child[1]->dataType != AST_DATATYPE_CHAR) {
                        fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s must be assigned to char or int.\n",node->lineNumber,node->symbol->text);
						semanticError++;
					}
				}
            }
			else if(node->symbol->dataType == SYMBOL_DATATYPE_POINTER) {
				if(node->child[1] != NULL) {
					if(node->child[1]->dataType != AST_DATATYPE_POINTER) {
                        fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s must be assigned to pointer.\n",node->lineNumber,node->symbol->text);
						semanticError++;
					}
				}
			}
            break;
        case AST_ARRAY:
			if(node->symbol->dataType == SYMBOL_DATATYPE_INT) {
				node->dataType = AST_DATATYPE_INT;
			}
			else if(node->symbol->dataType == SYMBOL_DATATYPE_CHAR) {
				node->dataType = AST_DATATYPE_CHAR;
			}
			else if(node->symbol->dataType == SYMBOL_DATATYPE_POINTER) {
				node->dataType = AST_DATATYPE_POINTER;
			}

			if(node->symbol->type != SYMBOL_VET) {
				fprintf(stderr,"SEMANTIC ERROR in line %d. Identifier %s is not a vector.\n",node->lineNumber,node->symbol->text);
                semanticError++;
				node->dataType = AST_DATATYPE_ERROR;
			}
			if(node->child[0]->dataType != AST_DATATYPE_INT && node->child[0]->dataType != AST_DATATYPE_CHAR) {
				fprintf(stderr,"SEMANTIC ERROR in line %d. Invalid index type in array, must be char or int.\n",node->lineNumber);
				semanticError++;
				node->dataType = AST_DATATYPE_ERROR;
			}
            break;
		case AST_FUNCCALL:
			if(node->symbol->dataType == SYMBOL_DATATYPE_INT) {
				node->dataType = AST_DATATYPE_INT;
			}
			else if(node->symbol->dataType == SYMBOL_DATATYPE_CHAR) {
				node->dataType = AST_DATATYPE_CHAR;
			}
			else if(node->symbol->dataType == SYMBOL_DATATYPE_POINTER) {
				node->dataType = AST_DATATYPE_POINTER;
			}
			else if(node->symbol->dataType == SYMBOL_DATATYPE_BOOL) {
				node->dataType = AST_DATATYPE_BOOL;
			}

			if(node->symbol->type != SYMBOL_FUN) {
				fprintf(stderr, "SEMANTIC ERROR in line %d. Identifier %s is not a function.\n",node->lineNumber,node->symbol->text);
                semanticError++;
				node->dataType = AST_DATATYPE_ERROR;
			}
            else {

                switch (functionValidation(nodeDeclared, node)) {
                    case 1:
                        break;
                    case 2:
                        fprintf(stderr, "SEMANTIC ERROR in line %d. Incompatible parameter type in function %s.\n",node->lineNumber,node->symbol->text);
                        semanticError++;
                        break;
                    case 3:
                        fprintf(stderr, "SEMANTIC ERROR in line %d. Invalid number of parameters in function %s. \n",node->lineNumber, node->symbol->text);
                        semanticError++;
                        break;
                    default:
                        break;
                }

            }
            break;
        case AST_READ:
			if(node->symbol->type != SYMBOL_VAR) {
				fprintf(stderr,"SEMANTIC ERROR in line %d. Symbol %s must be scalar\n",node->lineNumber, node->symbol->text);
				semanticError++;
			}
			break;
		case AST_PRINT_PARAM:
			if(node->child[0] != NULL) {
                if(node->child[0]->type != AST_PRINT_PARAM) {
                    if(node->child[0]->dataType != AST_DATATYPE_STRING &&
                       node->child[0]->dataType != AST_DATATYPE_INT &&
                       node->child[0]->dataType != AST_DATATYPE_BOOL &&
                       node->child[0]->dataType != AST_DATATYPE_POINTER &&
                       node->child[0]->dataType != AST_DATATYPE_CHAR) {
                        semanticError++;
                    }
                }
			}
            break;
        case AST_IFT:
		case AST_IFTE:
		case AST_WHILE:
			if(node->child[0] != NULL) {
				if(node->child[0]->dataType != AST_DATATYPE_BOOL) {
					fprintf(stderr,"SEMANTIC ERROR in line %d. Condition type must be bool.\n",node->lineNumber);
                    semanticError++;
				}
			}
            break;
		case AST_ADD:
		case AST_SUB:
            if(node->child[0] != NULL && node->child[1] != NULL) {
                if (node->child[0]->dataType == AST_DATATYPE_POINTER) {
                    node->dataType = AST_DATATYPE_POINTER;
                    if (node->child[1]->symbol->type != SYMBOL_LIT_INT) {
                        fprintf(stderr,"SEMANTIC ERROR in line %d. Pointers can only be added or subtracted by constants.\n",node->lineNumber);
                        semanticError++;
                    }
                } else if (node->child[1]->dataType == AST_DATATYPE_POINTER) {
                    node->dataType = AST_DATATYPE_POINTER;
                    if (node->child[0]->symbol->type != SYMBOL_LIT_INT) {
                        fprintf(stderr,"SEMANTIC ERROR in line %d. Pointers can only be added or subtracted by constants.\n",node->lineNumber);
                        semanticError++;
                    }
                } else {
					if(node->child[0]->dataType == AST_DATATYPE_INT || node->child[1]->dataType == AST_DATATYPE_INT) {
						node->dataType = AST_DATATYPE_INT;
					}
					else {
						node->dataType = AST_DATATYPE_CHAR;
					}
					if((node->child[0]->dataType != AST_DATATYPE_INT && node->child[0]->dataType != AST_DATATYPE_POINTER) ||
						(node->child[1]->dataType != AST_DATATYPE_INT && node->child[1]->dataType != AST_DATATYPE_POINTER)) {
						fprintf(stderr,"SEMANTIC ERROR in line %d. Operators must be int, or char. \n", node->lineNumber);
						semanticError++;
						node->dataType = AST_DATATYPE_ERROR;
					}
                }
            }
            break;
		case AST_MULT:
		case AST_DIV:
			if(node->child[0] != NULL && node->child[1] != NULL) {
				if(node->child[0]->dataType == AST_DATATYPE_POINTER || node->child[1]->dataType == AST_DATATYPE_POINTER) {
                    fprintf(stderr,"SEMANTIC ERROR in line %d. Operators must be int, pointer, char or bool. \n", node->lineNumber);
                    semanticError++;
                    node->dataType = AST_DATATYPE_ERROR;
				}
				else {
					if(node->child[0]->dataType == AST_DATATYPE_INT || node->child[1]->dataType == AST_DATATYPE_INT) {
						node->dataType = AST_DATATYPE_INT;
					}
					else {
						node->dataType = AST_DATATYPE_CHAR;
					}
					if((node->child[0]->dataType != AST_DATATYPE_INT && node->child[0]->dataType != AST_DATATYPE_POINTER) ||
						(node->child[1]->dataType != AST_DATATYPE_INT && node->child[1]->dataType != AST_DATATYPE_POINTER)) {
						fprintf(stderr,"SEMANTIC ERROR in line %d. Operators must be int, or char. \n", node->lineNumber);
						semanticError++;
						node->dataType = AST_DATATYPE_ERROR;
					}
				}
			}
            break;
		case AST_LESS:
		case AST_GREATER:
		case AST_LE:
		case AST_GE:
		case AST_EQ:
		case AST_DIF:
			if(node->child[0] != NULL && node->child[1] != NULL) {
				node->dataType = AST_DATATYPE_BOOL;
				if((node->child[0]->dataType != AST_DATATYPE_INT
                    && node->child[0]->dataType != AST_DATATYPE_POINTER
                    && node->child[0]->dataType != AST_DATATYPE_CHAR
                    && node->child[0]->dataType != AST_DATATYPE_BOOL) ||
                   (node->child[1]->dataType != AST_DATATYPE_INT
                    && node->child[1]->dataType != AST_DATATYPE_POINTER
                    && node->child[1]->dataType != AST_DATATYPE_CHAR
                    && node->child[1]->dataType != AST_DATATYPE_BOOL)) {
                    fprintf(stderr,"SEMANTIC ERROR in line %d. Operators must be int, pointer, char or bool.\n",
                            node->lineNumber);
						semanticError++;
						node->dataType = AST_DATATYPE_ERROR;
				}
			}
            break;
		case AST_AND:
		case AST_OR:
			if(node->child[0] != NULL && node->child[1] != NULL) {
				node->dataType = AST_DATATYPE_BOOL;
				if(node->child[0]->dataType != AST_DATATYPE_BOOL || node->child[1]->dataType != AST_DATATYPE_BOOL) {
						fprintf(stderr,"SEMANTIC ERROR in line %d. Operators must be bool.\n %d\n %d\n",node->lineNumber, node->child[0]->dataType, node->child[1]->dataType);
						semanticError++;
						node->dataType = AST_DATATYPE_ERROR;
				}
			}
            break;
		case AST_NOT:
			if(node->child[0] != NULL) {
				node->dataType = AST_DATATYPE_BOOL;
				if(node->child[0]->dataType != AST_DATATYPE_BOOL) {
						fprintf(stderr,"SEMANTIC ERROR in line %d. Operators must be bool.\n",node->lineNumber);
						semanticError++;
						node->dataType = AST_DATATYPE_ERROR;
				}
			}
			break;
        case AST_DOLLAR:
            if(node->child[0] != NULL) {
                node->dataType = AST_DATATYPE_POINTER;
            }
            break;
        case AST_HASH:
            if(node->child[0] != NULL) {
                node->dataType = AST_DATATYPE_INT;
                if(node->child[0]->dataType != AST_DATATYPE_POINTER) {
                    fprintf(stderr,"SEMANTIC ERROR in line %d. Operators must be pointer.\n",node->lineNumber);
                    semanticError++;
                    node->dataType = AST_DATATYPE_ERROR;
                }
            }
		default:
            break;
        }
}

void checkSemantic(AST* node) {
    fprintf(stderr, "\n");
	setAndCheckRedeclared(node);
	hashCheckUndeclared();
	checkOperands(node);
}

// ------------------------ FUNÇÕES AUXILIARES ------------------------

void setTypes(AST* node) {
	if(node->child[0] != NULL) {
        if(node->child[0]->type == AST_INT) {
            node->child[0]->dataType = AST_DATATYPE_INT;
			node->symbol->dataType = SYMBOL_DATATYPE_INT;
			node->dataType = AST_DATATYPE_INT;
		}
        else if(node->child[0]->type == AST_POINTER) {
            node->child[0]->dataType = AST_DATATYPE_POINTER;
			node->symbol->dataType = SYMBOL_DATATYPE_POINTER;
			node->dataType = AST_DATATYPE_POINTER;
		}
		else if(node->child[0]->type == AST_CHAR) {
            node->child[0]->dataType = AST_DATATYPE_CHAR;
			node->symbol->dataType = SYMBOL_DATATYPE_CHAR;
			node->dataType = AST_DATATYPE_CHAR;
		}
        else if(node->child[0]->type == AST_BOOL) {
            node->child[0]->dataType = AST_DATATYPE_BOOL;
			node->symbol->dataType = SYMBOL_DATATYPE_BOOL;
			node->dataType = AST_DATATYPE_BOOL;
		}
	}
}

int numParamsFunc(AST* node) {
	if(node == 0)
        return 0;
	else
		return 1 + numParamsFunc(node->child[1]);
	return 0;
}

int checkVet(AST* node, int dataType) {
	if(node != NULL) {
		if(((node->child[0]->symbol->dataType != dataType) ||
        (dataType == SYMBOL_DATATYPE_INT || dataType == SYMBOL_DATATYPE_CHAR)) && (
        (node->child[0]->symbol->dataType != SYMBOL_DATATYPE_INT && node->child[0]->symbol->dataType != SYMBOL_DATATYPE_CHAR)))
			return 0;
		if(node->child[1] != NULL)
			return checkVet(node->child[1], dataType);
	}
	return 1;
}

int getType(AST* node) {
    int op1, op2;

    switch(node->type) {
        case AST_SYMBOL:
        case AST_ARRAY:
        case AST_FUNCCALL:
            node->dataType = node->symbol->dataType;
            break;
        case AST_PARENTH:
            node->dataType = getType(node->child[0]);
            break;
        case AST_ADD:
        case AST_SUB:
        case AST_DIV:
        case AST_MULT:
            op1 = getType(node->child[0]);
            op2 = getType(node->child[1]);
            node->dataType = expressionTypes(op1,op2);
            break;
        case AST_LE:
        case AST_GE:
        case AST_EQ:
        case AST_DIF:
        case AST_AND:
        case AST_OR:
        case AST_GREATER:
        case AST_LESS:
        case AST_NOT:
            node->dataType = SYMBOL_DATATYPE_BOOL;
        break;
    }

    return node->dataType;
}

int expressionTypes(int op1,int op2) {
    switch (op1) {
    case SYMBOL_DATATYPE_BOOL:
        switch (op2) {
        case SYMBOL_DATATYPE_BOOL:
            return SYMBOL_DATATYPE_BOOL;

        default:
            return SYMBOL_DATATYPE_ERROR;
        }

    case SYMBOL_DATATYPE_INT:
        switch (op2) {
        case SYMBOL_DATATYPE_BOOL:
        case SYMBOL_DATATYPE_INT:
        case SYMBOL_DATATYPE_CHAR:
            return SYMBOL_DATATYPE_INT;

        case SYMBOL_DATATYPE_POINTER:
            return SYMBOL_DATATYPE_POINTER;

        default: return SYMBOL_DATATYPE_ERROR;

        }

    case SYMBOL_DATATYPE_CHAR:
        switch (op2) {
        case SYMBOL_DATATYPE_BOOL:
        case SYMBOL_DATATYPE_INT:
            return SYMBOL_DATATYPE_INT;

        case SYMBOL_DATATYPE_CHAR:
            return SYMBOL_DATATYPE_CHAR;

        case SYMBOL_DATATYPE_POINTER:
            return SYMBOL_DATATYPE_POINTER;

        default: return SYMBOL_DATATYPE_ERROR;

        }

    case SYMBOL_DATATYPE_POINTER:
        switch (op2) {
        case SYMBOL_DATATYPE_BOOL:
            return SYMBOL_DATATYPE_ERROR;

        case SYMBOL_DATATYPE_ERROR:
            return SYMBOL_DATATYPE_ERROR;

        default: return SYMBOL_DATATYPE_POINTER;
        }

    default: return SYMBOL_DATATYPE_ERROR;
    }
}


int functionValidation(AST *nodeDeclared, AST *node) {
    AST *aux = nodeDeclared;

    if ((nodeDeclared->type == AST_DECFUNC || nodeDeclared->type == AST_DECFUNC_VOID) && strcmp(nodeDeclared->symbol->text, node->symbol->text) == 0) {
        AST *declaration_param = NULL;
        int dec_type, call_type;

        if (nodeDeclared->type == AST_DECFUNC) {
            declaration_param = nodeDeclared->child[1];
        }

        AST *funccall_param = node->child[0];

        if (declaration_param == NULL && funccall_param == NULL)
            return 1;
        if (!checkParams(node))
            return 3;

        while (declaration_param != NULL && funccall_param != NULL) {
            switch (declaration_param->child[0]->type) {
            case AST_INT:
                dec_type = SYMBOL_DATATYPE_INT;
                break;

            case AST_POINTER:
                dec_type = SYMBOL_DATATYPE_POINTER;
                break;

            case AST_CHAR:
                dec_type = SYMBOL_DATATYPE_CHAR;
                break;

            case AST_BOOL:
                dec_type = SYMBOL_DATATYPE_BOOL;
                break;

            default:
                dec_type = SYMBOL_DATATYPE_ERROR;
                break;
            }

            call_type = funccall_param->child[0]->dataType;

            if (dec_type != call_type) {
                if ((dec_type == SYMBOL_DATATYPE_CHAR && call_type == SYMBOL_DATATYPE_INT) || (dec_type == SYMBOL_DATATYPE_INT && call_type == SYMBOL_DATATYPE_CHAR))
                    return 1;
                else
                    return 2;
            }
            if (declaration_param->child[1])
                declaration_param = declaration_param->child[1]->child[0];

            else {
                declaration_param = NULL;
            }

            if (funccall_param->child[1]) {
                funccall_param = funccall_param->child[1]->child[0];
            }
            else {
                funccall_param = NULL;
            }
        }


    }
    else {
        int numberChildren = 0;

        for (numberChildren = 0; numberChildren < 4; numberChildren++)
        {
            if (nodeDeclared->child[numberChildren] != NULL) {
                int found;
                found = functionValidation(nodeDeclared->child[numberChildren], node);
                if (found != 5)
                    return found;
            }
        }
    }

    return 5;
}

int validReturn(AST *nodeDec,AST *node) {
    int decType=nodeDec->symbol->dataType;
    int returnType;
    if (node->type==AST_RETURN) {
        returnType=node->child[0]->dataType;

        if(decType!=returnType)
            if ((decType == SYMBOL_DATATYPE_CHAR && returnType == SYMBOL_DATATYPE_INT) || (decType == SYMBOL_DATATYPE_INT && returnType == SYMBOL_DATATYPE_CHAR))
                    return 1;

       		 else return 0;

        else return 1;
    }
    else {
        int numberChildren = 0;

        for (numberChildren = 0; numberChildren < 4; numberChildren++) {
            if (node->child[numberChildren] != NULL) {
                int found;
                found = validReturn(nodeDec,node->child[numberChildren]);
                if (found != 5)
                    return found;
            }
        }
    }

    return 5;

}

int checkParams(AST* node) {
	int countParams = 0;

	int params = node->symbol->numParams;

	countParams = numParamsFunc(node->child[0]);

	if(countParams != params) {
		return 0;
	}

    return 1;
}
