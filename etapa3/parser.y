%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "hash.h"

    int yyerror (char *msg);
    int yylex(void);
    int getLineNumber(void);
%}

%token KW_CHAR
%token KW_INT
%token KW_BOOL
%token KW_POINTER

%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF
%token LEFT_ASSIGN
%token RIGHT_ASSIGN
%token TK_IDENTIFIER

%token LIT_INTEGER
%token LIT_TRUE
%token LIT_FALSE
%token LIT_CHAR
%token LIT_STRING

%token TOKEN_ERROR

%left '|' '&'
%left '<' '>' OPERATOR_EQ OPERATOR_LE OPERATOR_GE OPERATOR_DIF
%left '+' '-'
%left '*' '/'

%%

program: declist
	;

declist: dec declist
	|
	;

dec: 
	type TK_IDENTIFIER ':' lit ';'
	| type TK_IDENTIFIER '('')'cmd_list
	| type TK_IDENTIFIER '('dec_param')' cmd_list
	| type '['LIT_INTEGER']' TK_IDENTIFIER array_init ';'
	;

dec_param:
	type TK_IDENTIFIER dec_param2
	;

dec_param2:
	',' dec_param
	|
	;

cmd_list: 
	cmd ';' cmd_list
	| 
	;

print_list:
	LIT_STRING ',' print_list
	| exp ',' print_list
	|LIT_STRING
	|exp
	;

cmd:
	TK_IDENTIFIER ':' exp 
	| TK_IDENTIFIER LEFT_ASSIGN exp
	| exp RIGHT_ASSIGN TK_IDENTIFIER
	| TK_IDENTIFIER '[' exp ']' LEFT_ASSIGN exp
	| exp RIGHT_ASSIGN TK_IDENTIFIER '[' exp ']'
	| KW_RETURN exp 
	| KW_READ TK_IDENTIFIER
	| KW_PRINT print_list 
	| block 
	| ctrl_flow
	|
	;

block:
	'{'cmd_list '}'
	;

ctrl_flow:
	KW_IF '(' exp ')' KW_THEN cmd
	| KW_IF '(' exp ')' KW_THEN cmd KW_ELSE cmd
	| KW_WHILE '(' exp ')' cmd
	;
	
lit:
	LIT_INTEGER
	| LIT_CHAR
	| LIT_TRUE
	| LIT_FALSE
	;

type:
	KW_INT
	| KW_BOOL
	| KW_POINTER
	| KW_CHAR
	;

array_init:
	':' lit array_init2
	|
	;

array_init2:
	lit array_init2
	|
	;

exp:
	TK_IDENTIFIER
	| TK_IDENTIFIER '[' exp ']'
	| TK_IDENTIFIER '(' func_param ')'
	| '(' exp ')'
	| lit
	| exp '+' exp
	| exp '-' exp
	| exp '*' exp
	| exp '/' exp
	| exp '>' exp
	| exp '<' exp
	| exp '|' exp
	| exp '&' exp
	| '~' exp
	| '$' exp
	| '#' exp
	| exp OPERATOR_LE exp
	| exp OPERATOR_GE exp
	| exp OPERATOR_EQ exp
	| exp OPERATOR_DIF exp
	;

func_param:
	exp func_param2
	|
	;

func_param2:
	',' func_param
	|
	;

%%

int yyerror (char *msg) {
    fprintf(stderr, "Syntax error at line %d \n", getLineNumber());
    exit(3);
}
