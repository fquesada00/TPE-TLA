%{
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "AST.h"
    #include "translateAST.h"
    #define MAX_IDENTIFIER_LENGTH 256
    #define YYDEBUG 0
    int yydebug = 0;
    void yyerror (char *s);
    int yylex();
    AstGraphNode * entrypoint;
%}
%start program
%union {struct AstNode * node;int num; char * string;}
%token GRAPH
%token <string> ID STRING_VALUE
%token <num> NUMBER
%token INT STRING
%type <node> blockcode code declaration exp
%type <num> term
%%
program:        GRAPH '(' ')' blockcode     {entrypoint = newAstGraphNode((AstBlockcodeNode *)$4); return 1;}
                ;
blockcode:      '{' code '}'                {$$ = (AstNode *) newAstBlockcodeNode((AstCodeNode *)$2);}
                ;
code:           ';'                         {$$ = (AstNode *) newAstCodeNode((AstNode *)NULL,(AstCodeNode *)NULL);}
                |
                declaration ';'             {$$ = (AstNode *) newAstCodeNode($1,(AstCodeNode *)NULL);}
                |
                code declaration ';'        {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                                            {/* empty */}
;
declaration:    INT ID                      {$$ = (AstNode *) newAstDeclarationNode(INT_DECLARATION_TYPE,(AstArithmeticExpressionNode *)NULL,$2);free($2);}
                |                
                INT ID '=' exp              {$$ = (AstNode *) newAstDeclarationNode(INT_DECLARATION_TYPE, (AstArithmeticExpressionNode *)$4, $2);free($2);}
                |
                STRING ID                   {$$ = (AstNode *) newAstDeclarationNode(STRING_DECLARATION_TYPE,(AstArithmeticExpressionNode *)NULL,$2);free($2);}
                |
                STRING ID '=' STRING_VALUE  {$$ = (AstNode *) newAstDeclarationNode(STRING_DECLARATION_TYPE,(AstArithmeticExpressionNode *)newAstConstantExpressionNode($4),$2);free($2);free($4)}
;
exp:            term                    {$$ = (AstNode *) newAstArithmeticExpressionNode((AstArithmeticExpressionNode *) NULL,(AstArithmeticExpressionNode *) NULL, (char *) NULL, $1);}
                |
                exp '+' exp             {$$ = (AstNode *) newAstArithmeticExpressionNode((AstArithmeticExpressionNode *) $1, (AstArithmeticExpressionNode *) $3, "+", 0);}
                |
                exp '-' exp             {$$ = (AstNode *) newAstArithmeticExpressionNode((AstArithmeticExpressionNode *) $1, (AstArithmeticExpressionNode *) $3, "-", 0);}
;
term:           NUMBER                  {$$ = $1;}			    
;

%%

int main (void) {
	yyparse();
    translateAstGraphNode(entrypoint);
    return 1;
}

void yyerror (char *s) {
    fprintf(stderr, "ERROR at %s(%s:%d)", __func__, __FILE__, __LINE__); 
    fprintf (stderr, "%s\n", s);
} 