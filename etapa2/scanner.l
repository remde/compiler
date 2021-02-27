%{ 
    #include <stdio.h>
    #include <stdlib.h>
    #include "y.tab.h"
    #include "hash.c"

    int isRunningFlag = 1;
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

[0-9]+                        { hashInsert(yytext, 0); return LIT_INTEGER; }

[a-z_A-Z]+[a-zA-Z_0-9]*       { if( yytext[-1]!='"') {
				  hashInsert(yytext, 0); 
                                  return TK_IDENTIFIER;
                                }
                              } 

'(\\.|[^'\n])*\'	      { if (yyleng==3) { 
				  hashInsert(yytext, 0);
                                  return LIT_CHAR;
                                }
                                else return TOKEN_ERROR;
                              } 
                
\"(\\.|[^\"\n])*\" 	      { hashInsert(yytext, 0); return LIT_STRING; }

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

"//".*  
"/*"		              { BEGIN(COMMENT); }
.	                      { return TOKEN_ERROR; }

<COMMENT>"*/"                 { BEGIN(INITIAL); }
<COMMENT>.                                      
<COMMENT>"\n"                 { ++line; }

%%

int getLineNumber(void) {   
    return line;
}

void initMe(void) {
    hashInit();
}

int isRunning(void) {
    return isRunningFlag;
}
    
int yywrap(void) {
    isRunningFlag = 0;
    return 1;
} 
