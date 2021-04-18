#include <stdio.h>
#include "hash.h"
#include "ast.h"

void setAndCheckRedeclared(AST *node);
void checkOperans(AST *node);
void checkSemantic(AST *node);
