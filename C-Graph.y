%{
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "AST.h"
    #include "translateAST.h"
    #define MAX_IDENTIFIER_LENGTH 256
    #define YYDEBUG 1
    int yydebug = 1;
    void yyerror (char *s);
    int yylex();
    int yylex_destroy();
    AstGraphNode * entrypoint;
%}
%start program
%union {struct AstNode * node;int num; char * string;}
%token GRAPH
%token <string> ID STRING_VALUE
%token <num> NUMBER
%token INT STRING
%token <string> OPERATOR 
%left OPERATOR
%type <node> blockcode code declaration exp
%type <num> term
%%
program:        GRAPH '(' ')' blockcode     {entrypoint = newAstGraphNode((AstBlockcodeNode *)$4); return 1;}
                ;
blockcode:      '{' code '}'                {$$ = (AstNode *) newAstBlockcodeNode((AstCodeNode *)$2);}
                ;
code:           ';'                         {$$ = (AstNode *) newAstCodeNode((AstNode *)NULL,(AstCodeNode *)NULL);}
                |
                code declaration ';'        {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                                            {$$ = NULL;}
;
declaration:    INT ID                      {$$ = (AstNode *) newAstDeclarationNode(INT_DECLARATION_TYPE,(AstNode *)NULL,$2);free($2);}
                |                
                INT ID '=' exp              {$$ = (AstNode *) newAstDeclarationNode(INT_DECLARATION_TYPE, (AstNode *)$4, $2);free($2);}
                |
                STRING ID                   {$$ = (AstNode *) newAstDeclarationNode(STRING_DECLARATION_TYPE,(AstNode *)NULL,$2);free($2);}
                |
                STRING ID '=' STRING_VALUE  {$$ = (AstNode *) newAstDeclarationNode(STRING_DECLARATION_TYPE,(AstNode *)newAstConstantExpressionNode($4),$2);free($2);free($4);}
;
exp:            term                        {$$ = (AstNode *) newAstArithmeticExpressionNode((AstArithmeticExpressionNode *) NULL,(AstArithmeticExpressionNode *) NULL, (char *) NULL, $1);}
                |
                exp OPERATOR exp            {$$ = (AstNode *) newAstArithmeticExpressionNode((AstArithmeticExpressionNode *) $1, (AstArithmeticExpressionNode *) $3, $2, 0);free($2);}
;
term:           NUMBER                      {$$ = $1;}			    
;

%%

int main (void) {
	yyparse();
    yylex_destroy();
    translateAstGraphNode(entrypoint);
    return 1;
}

void yyerror (char *s) {
    fprintf (stderr, "%s\n", s);
    exit(EXIT_FAILURE);
} 