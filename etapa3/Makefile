etapa3: y.tab.o lex.yy.o ast.o main.o
	gcc lex.yy.o y.tab.o ast.o main.o -o etapa3

main.o: main.c
	gcc -c main.c

ast.o: ast.c
	gcc -c ast.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

y.tab.c: parser.y 
	yacc -d -v parser.y

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm *.o lex.yy.c y.tab.c y.tab.h y.output etapa3