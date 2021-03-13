etapa2: y.tab.o lex.yy.o main.o
	gcc y.tab.o lex.yy.o main.o -o etapa2

main.o: main.c
	gcc -c main.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

y.tab.c: parser.y
	yacc -d -v parser.y

lex.yy.c: scanner.l
	lex scanner.l 

clean:
	rm *.o lex.yy.c y.tab.c y.tab.h y.output etapa2