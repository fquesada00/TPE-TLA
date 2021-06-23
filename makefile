all:
	yacc -d C-Graph.y --debug --verbose
	lex C-Graph.l
	gcc -o CG lex.yy.c y.tab.c -lfl

clean:
	rm CG y.tab.c y.tab.h lex.yy.c
