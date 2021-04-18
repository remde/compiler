etapa5: y.tab.o lex.yy.o ast.o semantic.o tacs.o main.o
	gcc lex.yy.o y.tab.o ast.o semantic.o tacs.o main.o -o etapa5

main.o: main.c
	gcc -c main.c

ast.o: ast.c
	gcc -c ast.c

semantic.o: semantic.c
	gcc -c semantic.c

tacs.o: tacs.c
	gcc -c tacs.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

y.tab.c: parser.y
	yacc -d -v parser.y

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm *.o lex.yy.c y.tab.c y.tab.h y.output etapa5
