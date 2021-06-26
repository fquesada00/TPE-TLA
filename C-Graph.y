%{
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "AST.h"
    #include "translateAST.h"
    #include "symbolTable.h"
    #define MAX_IDENTIFIER_LENGTH 256
    #define YYDEBUG 0
    int yydebug = 0;
    void yyerror (char *s);
    int yylex();
    int yylex_destroy();
    ScopeTable * scopeTable;
    AstGraphNode * entrypoint;
%}
%start program
%union {struct AstNode * node;int num; char * string;}
%token GRAPH

%token <string> ID STRING_VALUE ELSE_IF IF ELSE
%token <num> NUMBER
%token INT STRING
%token <string> OPERATOR 
%token <string> BINARY_BOOL_OPERATOR
%token <string> UNARY_BOOL_OPERATOR
%left OPERATOR BINARY_BOOL_OPERATOR UNARY_BOOL_OPERATOR
%type <node> blockcode code declaration exp boolExp conditional conditionalElse definition
%type <num> term
%%
program:        GRAPH '(' ')' blockcode     {entrypoint = newAstGraphNode((AstBlockcodeNode *)$4); return 1;}
                ;
blockcode:      '{' { pushScope(scopeTable); } code '}'                {$$ = (AstNode *) newAstBlockcodeNode((AstCodeNode *)$3);popScope(scopeTable);}
                ;
code:           ';'                         {$$ = (AstNode *) newAstCodeNode((AstNode *)NULL,(AstCodeNode *)NULL);}
                |
                code declaration ';'        {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code conditional            {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code definition ';'         {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                                            {$$ = (AstNode *) NULL;}
;
declaration:    INT ID                      {
                                                if(findSymbol(scopeTable,$2)){
                                                    yyerror("Cannot redeclare variable");
                                                }else 
                                                    addSymbol(scopeTable,$2,INT_DECLARATION_TYPE);
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)NULL,$2,INT_DECLARATION_TYPE);
                                                free($2);
                                            }
                |                
                INT ID '=' exp              {
                                                if(findSymbol(scopeTable,$2)){
                                                    yyerror("Cannot redeclare variable");
                                                }else
                                                    addSymbol(scopeTable,$2,INT_DECLARATION_TYPE);
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)$4, $2,INT_DECLARATION_TYPE);
                                                free($2);
                                                }
                |
                STRING ID                   {
                                                if(findSymbol(scopeTable,$2)){
                                                        yyerror("Cannot redeclare variable");
                                                }else
                                                    addSymbol(scopeTable,$2,STRING_DECLARATION_TYPE);
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)NULL,$2,STRING_DECLARATION_TYPE);
                                                free($2);
                                            }
                |
                STRING ID '=' STRING_VALUE  {   
                                                if(findSymbol(scopeTable,$2)){
                                                        yyerror("Cannot redeclare variable");
                                                }else
                                                    addSymbol(scopeTable,$2,STRING_DECLARATION_TYPE);
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)newAstConstantExpressionNode($4),$2,STRING_DECLARATION_TYPE);
                                                free($2);
                                                free($4);
                                            }
;
definition:     ID '=' exp                  {
                                                Symbol * symbol;
                                                if((symbol=findSymbol(scopeTable,$1)) != NULL){
                                                    if(symbol->dataType != INT_DECLARATION_TYPE)
                                                        yyerror("Invalid definition data type");
                                                }else
                                                    yyerror("Cannot redeclare variable");
                                                $$ = (AstNode *) newAstDefinitionNode($3,$1,INT_DECLARATION_TYPE);
                                                free($1);
                                            }
                |
                ID '=' STRING_VALUE         {
                                                Symbol * symbol;
                                                if((symbol=findSymbol(scopeTable,$1)) != NULL){
                                                    if(symbol->dataType != STRING_DECLARATION_TYPE)
                                                        yyerror("Invalid definition data type");
                                                }else
                                                    yyerror("Cannot redeclare variable");
                                                $$ = (AstNode *) newAstDefinitionNode((AstNode *)newAstConstantExpressionNode($3),$1,STRING_DECLARATION_TYPE);
                                                free($1);
                                            }
;
exp:            term                        {$$ = (AstNode *) newAstArithmeticExpressionNode((AstArithmeticExpressionNode *) NULL,(AstArithmeticExpressionNode *) NULL, (char *) NULL, $1);}
                |
                exp OPERATOR exp            {$$ = (AstNode *) newAstArithmeticExpressionNode((AstArithmeticExpressionNode *) $1, (AstArithmeticExpressionNode *) $3, $2, 0);free($2);}
;
term:           NUMBER                      {$$ = $1;}			    
;
conditional:    IF '(' boolExp ')' blockcode conditionalElse    {$$ = (AstNode *) newAstIfNode((AstBooleanExpressionNode *)$3,(AstBlockcodeNode * )$5,IF_TYPE, (AstIfNode *) $6);}                           
;
conditionalElse:   
                ELSE_IF '(' boolExp ')' blockcode conditionalElse  {$$ = (AstNode *) newAstIfNode((AstBooleanExpressionNode *)$3,(AstBlockcodeNode * )$5, ELSE_IF_TYPE, (AstIfNode *) $6);}
                |
                ELSE blockcode                                      {$$ = (AstNode *) newAstIfNode((AstBooleanExpressionNode *) NULL,(AstBlockcodeNode * )$2,ELSE_IF_TYPE, (AstIfNode *) NULL);}
                |
                                                                    {$$ = (AstNode *) NULL;}
;

boolExp:        term                              {$$ = (AstNode *) newAstBooleanExpressionNode(NULL,NULL,NULL,$1);}
                |
                boolExp BINARY_BOOL_OPERATOR boolExp {$$ = (AstNode *) newAstBooleanExpressionNode((AstBooleanExpressionNode *)$1,(AstBooleanExpressionNode *)$3,$2,0);free($2);}
                |
                UNARY_BOOL_OPERATOR boolExp       {$$ = (AstNode *) newAstBooleanExpressionNode(NULL,(AstBooleanExpressionNode *)$2,$1,0);free($1);}
;
%%


int main (void) {
    scopeTable = createScopeTable();
	yyparse();
    yylex_destroy();
    free(scopeTable);
    translateAstGraphNode(entrypoint);
    return 1;
}

void yyerror (char *s) {
    fprintf (stderr, "%s\n", s);
    exit(EXIT_FAILURE);
} 