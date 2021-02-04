

%%

int		{printf("KW 'int'\n");}
for		{printf("KW 'for'\n");}
0*		{printf("sequence of 0s\n");}
[a-zA-Z]+	{printf("identifier\n");}
"+"		{printf("sum operator\n");}
[ \n\t]
.		{printf("invalid character\n");}


%%

int main() {
  yylex();
}

int yywrap() {
  return 1;
}
