all:
	yacc -d C-Graph.y --debug --verbose
	lex C-Graph.l
	$(CC) -o CG lex.yy.c y.tab.c AST.c translateAST.c -lfl -g

clean:
	rm CG y.tab.c y.tab.h lex.yy.c y.output

graph:
	$(CC) -o graph graph.c  -Wall -pedantic -g

.PHONY: all clean graph