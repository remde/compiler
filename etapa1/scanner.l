%{ 
    #include <stdio.h>
    #include <stdlib.h>
    #include "tokens.h"
    #include "hash.c"
    #include "main.c"

    int isRunning = 1;
    int line = 1;
%}

%x COMMENT

%%

"char"			      { return KW_CHAR; }
"int"                         { return KW_INT; }
"bool"			      { return KW_BOOL; }
"pointer"		      { return KW_POINTER; }
"if"                          { return KW_IF; }
"then"                        { return KW_THEN; }
"else"                        { return KW_ELSE; }
"while"			      { return KW_WHILE; }
"read"                        { return KW_READ; }
"return"                      { return KW_RETURN ; }
"print"                       { return KW_PRINT; }

"true"			      { return LIT_TRUE; }
"false"			      { return LIT_FALSE; }

[a-z_0-9]+                    { if( yytext[-1]!='"') {
				  hashInsert(0,yytext); 
                                  return TK_IDENTIFIER;
                                }
                              } 
                                  
[A-IO]+                       { hashInsert(0,yytext); return LIT_INTEGER; }

'(\\.|[^'\n])*\'	      { if (yyleng==3) { 
				  hashInsert(0,yytext);
                                  return LIT_CHAR;
                                }
                                else return TOKEN_ERROR;
                              } 
                
\"(\\.|[^\"\n])*\" 	      { hashInsert(0,yytext); return LIT_STRING; }

[-+*/|(){}<>;,:&$#~]          { return (int)yytext[0]; }
"["                           { return (int)yytext[0]; }
"]"                           { return (int)yytext[0]; }

"<="                          { return OPERATOR_LE; }
">="                          { return OPERATOR_GE; }
"=="                          { return OPERATOR_EQ; }
"!="                          { return OPERATOR_DIF; }
"<-"			      { return LEFT_ASSIGN; }
"->"			      { return RIGHT_ASSIGN; }

[ \t]
"\n"			      { ++line; }

"ZZ".*  
"XXX"		              { BEGIN(COMMENT); }
.	                      { return TOKEN_ERROR; }

%%

#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "hash.c"
#include "main.c"

int getLineNumber(void) {   
    return line;
}

void initMe(void) {
    hashInit();
}

int isRunning(void) {
    return isRunning;
}
    
int yywrap(void) {
    isRunning = 0;
    return 1;
} 
