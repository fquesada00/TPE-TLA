all:
	yacc -d C-Graph.y --debug --verbose
	lex C-Graph.l
	gcc -o CG lex.yy.c y.tab.c AST.c translateAST.c -lfl -g

clean:
	rm CG y.tab.c y.tab.h lex.yy.c y.output
