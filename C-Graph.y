%{
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "C-Graph.h"
    #define MAX_IDENTIFIER_LENGTH 256
    #define YYDEBUG 1
    // int yydebug = 1;
    void yyerror (char *s);
    int yylex();
    typedef struct symbol{
        char * name;
        data_type type;
        void * value;
        size_t size;
    } symbol;
    symbol symbol_table[16];
    int symbol_count = 0;
    void * get_symbol_value(char * name);
    void update_symbol(char * name,void * value);
    void * get_symbol_value(char * name);
    void create_symbol(data_type type, char * name);
%}
%union {int integer; char * string; int type;}
%start PROGRAM
%token <integer> graph
%token <string> identifier
%token <integer> number
%token <type> identifier_type
%type <integer> EXP TERM
%type <string> DEFINITION DECLARATION
%%
PROGRAM:        graph '(' ')' BLOCKCODE
                ;
BLOCKCODE:      '{' CODE '}'          
                ;
CODE:           ';'
                |
                DECLARATION ';'
                |
                DEFINITION ';'
                |
                CODE DECLARATION ';'
                |
                CODE DEFINITION ';'
                |
                CODE ';'
                |
                                                {/* empty */}
;
DECLARATION:    identifier_type identifier    {$$ = $2; create_symbol($1,$2);}
;
DEFINITION:     DECLARATION '=' EXP     {update_symbol($1,$3);}
                |
                identifier '=' EXP      {update_symbol($1,$3);}
;
EXP:            TERM                    {$$ = $1;}
                |
                EXP '+' TERM            {$$ = $1 + $3;}
                |
                EXP '-' TERM            {$$ = $1 - $3;}
;
TERM:           number                  {$$ = $1;}
		        | 
                identifier			    {$$ = get_symbol_value($1);} 
;
%%


void create_symbol(data_type type, char * name) {
    symbol_table[symbol_count].type = type;
    strcpy(symbol_table[symbol_count].name, name);
    size_t size;
    switch(type){
        case INT:
            size = sizeof(int);
            symbol_table[symbol_count].value = malloc(size);
            symbol_table[symbol_count].size = size;
        break;
        case STRING:
        break;
        default:
        break;
    }
    symbol_count++;
}

int get_symbol_index(char *name) {
    for(int i = 0 ; i < symbol_count; i++){
        if(strcmp(name, symbol_table[i].name)==0)
            return i;
    }
    return -1;
}

void update_symbol(char * name,void * value){
    int i = get_symbol_index(name);
    if(i ==-1) 
        exit(EXIT_FAILURE);
    switch(symbol_table[i].type){
        case INT:
            symbol_table[i].value = (int) value;
        break;
        case STRING:
            symbol_table[i].value = value;
        break;
        default:
        break;
    }

}

void * get_symbol_value(char * name){
    int i = get_symbol_index(name);
    if(i == -1)
        exit(EXIT_FAILURE);
    return symbol_table[i].value;
}

int main (void) {
	/* init symbol table */
	int i;
	for(i=0; i<16; i++) {
		memset(&symbol_table[i], 0, sizeof(symbol));
        symbol_table->name = malloc(sizeof(char) * MAX_IDENTIFIER_LENGTH);
	}

	return yyparse ( );
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 