all:
	cd Grammar; make all;
	cd ..;
	$(CC) -o CG Grammar/lex.yy.c Grammar/y.tab.c Grammar/Translate/AST.c  Grammar/Translate/translateAST.c  Grammar/symbolTable.c -lfl -g

clean:
	cd Grammar; make clean;
	cd ..;
	cd executables; rm -f *; cd ..;
	cd generatedCode; rm -f *; cd ..;
	rm -f CG

graph:
	$(CC) -o graph graph.c  -Wall -pedantic -g

.PHONY: all clean graph