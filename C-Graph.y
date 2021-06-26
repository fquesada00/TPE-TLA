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

%token <string> ID STRING_VALUE ELSE_IF IF ELSE FOR
%token <num> NUMBER
%token INT STRING
%token <string> OPERATOR 
%token <string> BINARY_BOOL_OPERATOR
%token <string> UNARY_BOOL_OPERATOR
%left OPERATOR BINARY_BOOL_OPERATOR UNARY_BOOL_OPERATOR
%type <node> blockcode code declaration exp boolExp conditional conditionalElse forLoop
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
                code conditional            {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code forLoop                {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                                            {$$ = (AstNode *) NULL;}
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
//output:         OUTPUT '(' STRING_VALUE ')'       {$$ = (AstNode *) newAstPrintNode($3);free($3);}
//                |
//                OUTPUT '(' ID ')'                 {$$ = (AstNode *) newAstPrintNode((AstDeclarationNode *)$3)->}
;              
forLoop:        FOR '(' declaration ';'  boolExp ';' declaration ')' blockcode {$$ = (AstNode *)newAstForNode((AstDeclarationNode *)$3,(AstBooleanExpressionNode *)$5,(AstDeclarationNode *)$7,(AstBlockcodeNode *)$9);}
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