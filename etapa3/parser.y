%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "hash.h"
	#include "ast.h"

    int yylex(void);
    int yyerror (char *msg);
    int getLineNumber(void);
%}

%union {
	HASH_NODE *symbol;
	AST *ast;
}

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

%token <symbol>TK_IDENTIFIER
%token <symbol>LIT_INTEGER
%token <symbol>LIT_TRUE
%token <symbol>LIT_FALSE
%token <symbol>LIT_CHAR
%token <symbol>LIT_STRING

%token TOKEN_ERROR

%type <ast> program
%type <ast> declist
%type <ast> dec
%type <ast> dec_param
%type <ast> dec_param2
%type <ast> cmd_list
%type <ast> print
%type <ast> print_list
%type <ast> cmd
%type <ast> block
%type <ast> ctrl_flow
%type <ast> lit
%type <ast> type
%type <ast> array_init
%type <ast> array_init2
%type <ast> exp
%type <ast> func_param
%type <ast> func_param2

%left '|' '&'
%left '<' '>' OPERATOR_EQ OPERATOR_LE OPERATOR_GE OPERATOR_DIF
%left '+' '-'
%left '*' '/'

%%

program: declist		{$$=$1; astPrint(0,$1); astDecompile($1);}
	;

declist: 
	dec declist			{$$=astCreate(AST_DEC,0,$1,$2,0,0);}	
	|					{$$=0;}
	;

dec: 
	type TK_IDENTIFIER ':' lit ';'							{$$=astCreate(AST_VARDEC,$2,$1,$4,0,0);}
	| type TK_IDENTIFIER '('')'cmd_list						{$$=astCreate(AST_DECFUNC_VOID,$2,$1,$5,0,0);}
	| type TK_IDENTIFIER '('dec_param')' cmd_list			{$$=astCreate(AST_DECFUNC,$2,$1,$4,$6,0);}
	| type '['LIT_INTEGER']' TK_IDENTIFIER array_init ';'	{$$=astCreate(AST_VETDEC,$5,$1,astCreate(AST_VECSIZE, $3, 0, 0, 0, 0),$6,0);}
	;

dec_param:
	type TK_IDENTIFIER dec_param2				{$$=astCreate(AST_DEC_PARAM,$2,$1,$3,0,0);}
	;

dec_param2:
	',' dec_param				{$$=astCreate(AST_DEC_PARAM2,0,$2,0,0,0);}
	|							{$$=0;}
	;

cmd_list: 
	cmd ';' cmd_list			{$$=astCreate(AST_LCMD,0,$1,$3,0,0);}
	| 							{$$=0;}
	;

print:
	print ',' print_list		{$$=astCreate(AST_PRINT_PARAM,0,$1,$3,0,0);}
	| print_list				{$$=astCreate(AST_PRINT_PARAM,0,$1,0,0,0);}
	;

print_list:
	LIT_STRING					{$$=astCreate(AST_SYMBOL,$1,0,0,0,0);}
	|exp						{$$=$1;}
	;

cmd:
	TK_IDENTIFIER ':' exp							{$$=astCreate(AST_ASSIGN,$1,$3,0,0,0);}
	| TK_IDENTIFIER LEFT_ASSIGN exp					{$$=astCreate(AST_ASSIGN,$1,$3,0,0,0);}
	| exp RIGHT_ASSIGN TK_IDENTIFIER				{$$=astCreate(AST_ASSIGN,$3,$1,0,0,0);}
	| TK_IDENTIFIER '[' exp ']' LEFT_ASSIGN exp		{$$=astCreate(AST_ASSIGNARRAY,$1,$3,$6,0,0);}
	| exp RIGHT_ASSIGN TK_IDENTIFIER '[' exp ']'	{$$=astCreate(AST_ASSIGNARRAY,$3,$5,$1,0,0);}
	| KW_RETURN exp									{$$=astCreate(AST_RETURN,0,$2,0,0,0);}
	| KW_READ TK_IDENTIFIER							{$$=astCreate(AST_READ,$2,0,0,0,0);}
	| KW_PRINT print	 							{$$=astCreate(AST_PRINT,0,$2,0,0,0);}
	| block 										{$$=$1;}
	| ctrl_flow										{$$=$1;}
	|												{$$=0;}
	;

block:
	'{'cmd_list '}'									{$$=astCreate(AST_BLOCK,0,$2,0,0,0);}
	;

ctrl_flow:
	KW_IF '(' exp ')' KW_THEN cmd					{$$=astCreate(AST_IFT,0,$3,$6,0,0);}
	| KW_IF '(' exp ')' KW_THEN cmd KW_ELSE cmd		{$$=astCreate(AST_IFTE,0,$3,$6,$8,0);}
	| KW_WHILE '(' exp ')' cmd						{$$=astCreate(AST_WHILE,0,$3,$5,0,0);}
	;
	
lit:
	LIT_INTEGER			{$$=astCreate(AST_SYMBOL,$1,0,0,0,0);}
	| LIT_CHAR			{$$=astCreate(AST_SYMBOL,$1,0,0,0,0);}
	| LIT_TRUE			{$$=astCreate(AST_SYMBOL,$1,0,0,0,0);}
	| LIT_FALSE			{$$=astCreate(AST_SYMBOL,$1,0,0,0,0);}
	;

type:
	KW_INT				{$$=astCreate(AST_INT,0,0,0,0,0);}
	| KW_BOOL			{$$=astCreate(AST_BOOL,0,0,0,0,0);}
	| KW_POINTER		{$$=astCreate(AST_POINTER,0,0,0,0,0);}
	| KW_CHAR			{$$=astCreate(AST_CHAR,0,0,0,0,0);}
	;

array_init:
	':' lit array_init2		{$$=astCreate(AST_ARRAY_INIT,0,$2,$3,0,0);}
	|						{$$=0;}
	;

array_init2:
	lit array_init2			{$$=astCreate(AST_ARRAY_INIT2,0,$1,$2,0,0);}
	|						{$$=0;}
	;

exp:
	TK_IDENTIFIER						{$$=astCreate(AST_SYMBOL,$1,0,0,0,0);}
	| TK_IDENTIFIER '[' exp ']'			{$$=astCreate(AST_ARRAY,$1,$3,0,0,0);}
	| TK_IDENTIFIER '(' func_param ')'	{$$=astCreate(AST_FUNCCALL,$1,$3,0,0,0);}
	| '(' exp ')'						{$$=astCreate(AST_PARENTH,0,$2,0,0,0);}
	| lit								{$$ = $1;}
	| exp '+' exp						{$$=astCreate(AST_ADD,0,$1,$3,0,0);}
	| exp '-' exp						{$$=astCreate(AST_SUB,0,$1,$3,0,0);}
	| exp '*' exp						{$$=astCreate(AST_MULT,0,$1,$3,0,0);}
	| exp '/' exp						{$$=astCreate(AST_DIV,0,$1,$3,0,0);}
	| exp '>' exp						{$$=astCreate(AST_GREATER,0,$1,$3,0,0);}
	| exp '<' exp						{$$=astCreate(AST_LESS,0,$1,$3,0,0);}
	| exp '|' exp						{$$=astCreate(AST_OR,0,$1,$3,0,0);}
	| exp '&' exp						{$$=astCreate(AST_AND,0,$1,$3,0,0);}
	| '~' exp							{$$=astCreate(AST_NOT,0,$2,0,0,0);}
	| '$' exp							{$$=astCreate(AST_DOLLAR,0,$2,0,0,0);}
	| '#' exp							{$$=astCreate(AST_HASH,0,$2,0,0,0);}
	| exp OPERATOR_LE exp				{$$=astCreate(AST_LE,0,$1,$3,0,0);}
	| exp OPERATOR_GE exp				{$$=astCreate(AST_GE,0,$1,$3,0,0);}
	| exp OPERATOR_EQ exp				{$$=astCreate(AST_EQ,0,$1,$3,0,0);}
	| exp OPERATOR_DIF exp				{$$=astCreate(AST_DIF,0,$1,$3,0,0);}
	;

func_param:
	exp func_param2			{$$=astCreate(AST_FUNC_PARAM,0,$1,$2,0,0);}
	|						{$$=0;}
	;

func_param2:
	',' func_param			{$$=astCreate(AST_FUNC_PARAM2,0,$2,0,0,0);}
	|						{$$=0;}
	;

%%

int yyerror (char *msg) {
    fprintf(stderr, "Syntax error at line %d \n", getLineNumber());
    exit(3);
}
