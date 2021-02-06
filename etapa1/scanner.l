%{ 
    #include <stdio.h>
    #include <stdlib.h>
    #include "tokens.h"
    #include "hash.c"


    int IsRunning = 1;
    int Line = 1;
%}

%x COMMENT

%%

"int"                         { return KW_INT; }
"if"                          { return KW_IF; }
"then"                        { return KW_THEN; }
"else"                        { return KW_ELSE; }
"read"                        { return KW_READ; }
"return"                      { return KW_RETURN ; }
"print"                       { return KW_PRINT; }

[a-z_0-9]+                    { if( yytext[-1]!='"')
                                { hashInsert(0,yytext); 
                                  return TK_IDENTIFIER;
                                }
                              } 
                                  
[A-IO]+                       { hashInsert(0,yytext); return LIT_INTEGER; }

'(\\.|[^'\n])*\'		      { if (yyleng==3)
                                {  hashInsert(0,yytext);
                                   return LIT_CHAR;
                                }
                                  else return TOKEN_ERROR;
                              } 
                
\"(\\.|[^\"\n])*\" 		{hashInsert(0,yytext); return LIT_STRING;}

[-+*/|(){}<>;,:&$#~]           { return (int)yytext[0]; }
"["                           { return (int)yytext[0]; }
"]"                           { return (int)yytext[0]; }

"<="                          { return OPERATOR_LE; }
">="                          { return OPERATOR_GE; }
"=="                          { return OPERATOR_EQ; }
"!="                          { return OPERATOR_DIF; }
"<-"			{ return LEFT_ASSIGN; }
"->"			{ return RIGHT_ASSIGN; }

[ \t]
"\n"			              { ++Line; }

"ZZ".*  
"XXX"			              { BEGIN(COMMENT); }
.		                      { return TOKEN_ERROR; }

%%

#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "hash.c"

int getLineNumber(void) {   
    return Line;
}

void initMe(void) {
    hashInit();
}

int isRunning(void) {
    return IsRunning;
}
    
int yywrap(void) {
    IsRunning = 0;
    return 1;
} 
