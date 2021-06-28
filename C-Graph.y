%{
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "AST.h"
    #include "translateAST.h"
    #include "symbolTable.h"
    #define MAX_IDENTIFIER_LENGTH 256
    #define YYDEBUG 1
    int yydebug = 1;
    void yyerror (char *s);
    int yylex();
    int yylex_destroy();
    ScopeTable * scopeTable;
    AstGraphNode * entrypoint;
    char c;
    int firstEntry = 1;
    extern int yylineno;
%}
%start program
%union {struct AstNode * node;int num; char * string;}
%token GRAPH

%token <string> ID STRING_VALUE ELSE_IF IF ELSE FOR
%token <num> NUMBER
%token INT STRING BFS DFS
%token <string> OPERATOR 
%token <string> BINARY_BOOL_OPERATOR
%token <string> UNARY_BOOL_OPERATOR
%token <string> INPUT OUTPUT
%token <string> NODE NODE_VALUE NODE_ID
%token <string> START_ARROW END_ARROW REMOVE_ADJACENCY REMOVE_NODE
%token <string> FOREACH NODE_ITERATOR IN EDGE_ITERATOR EDGE_WEIGHT
%token <string> TRAVERSE WITH TRAVERSE_ROCEDURE STARTING
%token COMMENT
%left OPERATOR BINARY_BOOL_OPERATOR UNARY_BOOL_OPERATOR
%type <node> blockcode code declaration exp boolExp conditional conditionalElse forLoop definition
%type <node> term forBlockcode output
%type <node> nodeDeclaration nextNodeDeclaration nodeValue
%type <node> adjacencyDeclaration edgeValue removeAdjacency removeGraphAdjacency removeGraphNode
%type <node> nodeBlockcode nodeCode foreachNode 
%type <node> edgeBlockcode edgeCode foreachEdge
%type <node> nodeAction nodeProperty graphAction edgeAction outputEdge outputNode
%type <node> traverseGraphNode traverseGraph traverseProcedure
%type <string> nodeField
%%
program:        GRAPH '(' ')' blockcode     {entrypoint = newAstGraphNode((AstBlockcodeNode *)$4); return 1;}
;
blockcode:      '{' { pushScope(scopeTable); if(firstEntry){ addSymbol(scopeTable,"nodeIterator", NODE_DECLARATION_TYPE);addSymbol(scopeTable,"edgeIterator", NODE_DECLARATION_TYPE);firstEntry = 0;} } code '}'                {$$ = (AstNode *) newAstBlockcodeNode((AstCodeNode *)$3);popScope(scopeTable);}
                ;
code:           ';'                         {$$ = (AstNode *) newAstCodeNode((AstNode *)NULL,(AstCodeNode *)NULL);}
                |
                code declaration ';'        {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code conditional            {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code forLoop                {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code definition ';'         {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code output ';'             {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code nodeDeclaration ';'    {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code adjacencyDeclaration ';'  {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code removeGraphAdjacency ';'    {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code removeGraphNode ';'    {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code foreachNode            {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code foreachEdge            {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code graphAction ';'        {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code traverseGraph ';'      {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code COMMENT
                |
                                            {$$ = (AstNode *) NULL;}
;
declaration:    INT ID                      {
                                                if(findSymbol(scopeTable,$2)){
                                                    yyerror("Syntax Error: Cannot redeclare variable.");
                                                }else 
                                                    addSymbol(scopeTable,$2,INT_DECLARATION_TYPE);
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)NULL,$2,INT_DECLARATION_TYPE);
                                                free($2);
                                            }
                |                
                INT ID '=' exp              {
                                                if(findSymbol(scopeTable,$2)){
                                                    yyerror("Syntax Error: Cannot redeclare variable.");
                                                }else
                                                    addSymbol(scopeTable,$2,INT_DECLARATION_TYPE);
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)$4, $2,INT_DECLARATION_TYPE);
                                                free($2);
                                                }
                |
                STRING ID                   {
                                                if(findSymbol(scopeTable,$2)){
                                                        yyerror("Syntax Error: Cannot redeclare variable.");
                                                }else
                                                    addSymbol(scopeTable,$2,STRING_DECLARATION_TYPE);
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)NULL,$2,STRING_DECLARATION_TYPE);
                                                free($2);
                                            }
                |
                STRING ID '=' STRING_VALUE  {   
                                                if(findSymbol(scopeTable,$2)){
                                                        yyerror("Syntax Error: Cannot redeclare variable.");
                                                }else
                                                    addSymbol(scopeTable,$2,STRING_DECLARATION_TYPE);
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)newAstConstantExpressionNode($4),$2,STRING_DECLARATION_TYPE);
                                                free($2);
                                                free($4);
                                            }
                |
                STRING ID '=' INPUT'(' ')'  {
                                                if(findSymbol(scopeTable,$2)){
                                                        yyerror("Syntax Error: Cannot redeclare variable.");
                                                }else
                                                    addSymbol(scopeTable,$2,INPUT_DECLARATION_TYPE);
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)NULL,$2,INPUT_DECLARATION_TYPE);
                                                free($2);
                                            }
                |
                STRING ID '=' ID            {
                                                Symbol * symbol;
                                                if((symbol = findSymbol(scopeTable,$4)) == NULL || findSymbol(scopeTable, $2) != NULL) {
                                                    yyerror("Syntax Error: Usage of undeclared variable.");
                                                }else{
                                                    addSymbol(scopeTable,$2,STRING_DECLARATION_TYPE);
                                                }
                                                if(symbol->dataType != INPUT_DECLARATION_TYPE || symbol->dataType != STRING_DECLARATION_TYPE){
                                                    yyerror("Syntax Error: Invalid data type, expected string data type.")
                                                }
                                                $$ = (AstNode *) newAstDeclarationNode((AstNode *)newAstConstantExpressionNode($4),$2,STRING_DECLARATION_TYPE);
                                                free($2);
                                                free($4);
                                            }
;
nodeDeclaration:    NODE ID '=' nodeValue nextNodeDeclaration   {   if(findSymbol(scopeTable,$2)){
                                                                            yyerror("Syntax Error: Cannot redeclare variable.");
                                                                    }else
                                                                        addSymbol(scopeTable,$2,NODE_DECLARATION_TYPE);
                                                                    $$ = (AstNode *) newAstGraphNodeDeclarationNode($2, (AstGraphNodeDeclarationNode *) $5, $4); 
                                                                    free($2);
                                                                }
;
nodeValue:          STRING_VALUE                {
                                                    $$ = (AstNode *)newAstConstantExpressionNode($1);
                                                    free($1);
                                                }
                    |
                    ID                          {
                                                    Symbol * symbol;
                                                    if((symbol=findSymbol(scopeTable,$1)) == NULL)
                                                        yyerror("Syntax Error: Usage of undeclared variable.");
                                                    if(symbol->dataType == INT_DECLARATION_TYPE || symbol->dataType == NODE_DECLARATION_TYPE ) 
                                                        yyerror("Invalid data type, expected string data type.");
                                                    $$ = (AstNode *)newAstIdNode($1, symbol->dataType);
                                                    free($1);
                                                }
;
nextNodeDeclaration:    ',' ID '=' nodeValue nextNodeDeclaration    {
                                                                        if(findSymbol(scopeTable,$2)){
                                                                            yyerror("Syntax Error: Cannot redeclare variable.");
                                                                        }else
                                                                            addSymbol(scopeTable,$2,NODE_DECLARATION_TYPE);
                                                                        $$ = (AstNode *) newAstGraphNodeDeclarationNode($2, (AstGraphNodeDeclarationNode *) $5, $4);
                                                                        free($2);
                                                                    }
                        |
                                                    {$$ = NULL;}
;
adjacencyDeclaration:   ID START_ARROW edgeValue END_ARROW ID       {
                                                                        if(!strncmp($1,$5,MAX_IDENTIFIER_LENGTH)){
                                                                            yyerror("Syntax Error: Self-Loops are not supported.");
                                                                        }
                                                                        Symbol * left, * right;    
                                                                        if((left = findSymbol(scopeTable,$1)) == NULL || (right = findSymbol(scopeTable,$5)) == NULL)
                                                                            yyerror("Syntax Error: Cannot redeclare variable.");
                                                                        if(left->dataType != NODE_DECLARATION_TYPE || right->dataType != NODE_DECLARATION_TYPE)
                                                                            yyerror("Syntax Error: Expected Node data type.");
                                                                        $$ = (AstNode *) newAstGraphEdgeDeclarationNode($1,$5,$3);
                                                                        free($1);
                                                                        free($5);
                                                                        free($2);
                                                                        free($4);
                                                                    }
;

removeGraphNode:        REMOVE_NODE ID                              {
                                                                        Symbol * symbol;
                                                                        if((symbol = findSymbol(scopeTable,$2))==NULL){
                                                                            yyerror("Syntax Error: Cannot delete undeclared node.");
                                                                        }else if(symbol->dataType != NODE_DECLARATION_TYPE)
                                                                            yyerror("Syntax Error: Expected Node data type");
                                                                        removeSymbol(scopeTable,$2);
                                                                        $$ = (AstNode *)newAstNodeRemoveNode($2);free($2);
                                                                    }
;
removeGraphAdjacency:   ID REMOVE_ADJACENCY ID                      {

                                                                        Symbol * left, * right;
                                                                        if((left = findSymbol(scopeTable,$1)) == NULL || (right = findSymbol(scopeTable,$1)) == NULL){
                                                                            yyerror("Syntax Error: Usage of undeclared node variable.");
                                                                        }
                                                                        if(left->dataType != NODE_DECLARATION_TYPE || right->dataType != NODE_DECLARATION_TYPE) {
                                                                            yyerror("Syntax Error: Expected Node data type.");
                                                                        }
                                                                        if(!strncmp($1,$3,MAX_IDENTIFIER_LENGTH)){
                                                                            yyerror("Syntax Error: Self-Loops are not supported.");
                                                                        }
                                                                        $$ = (AstNode*)newAstEdgeRemoveNode($1,$3);
                                                                        free($1);
                                                                        free($3);
                                                                                                                                    
                                                                    }
;
removeAdjacency:        nodeField REMOVE_ADJACENCY nodeField        {
                                                                        if(!strncmp($1,$3,MAX_IDENTIFIER_LENGTH)){
                                                                            yyerror("Syntax Error: Self-Loops are not supported.");
                                                                        }

                                                                        $$ = (AstNode*)newAstEdgeRemoveNode($1,$2);
                                                                        free($1);
                                                                        free($2);
                                                                    }
;
edgeValue:              NUMBER                                      { $$ = (AstNode *) newAstNumericExpressionNode($1);}
                        |
                        STRING_VALUE                                { $$ = (AstNode *) newAstConstantExpressionNode($1);free($1);}
                        |
                        ID                                          {
                                                                        Symbol * symbol;
                                                                        if((symbol=findSymbol(scopeTable,$1)) == NULL)
                                                                            yyerror("Syntax Error: Usage of undeclared variable.");
                                                                        if(symbol->dataType == NODE_DECLARATION_TYPE ) 
                                                                            yyerror("Syntax Error: Invalid data type. Edge definition only allows int and string type for weight.");
                                                                        $$ = (AstNode *)newAstIdNode($1, symbol->dataType);
                                                                        free($1);
                                                                    }                        
;

foreachNode:            FOREACH '(' NODE_ITERATOR IN GRAPH ')' nodeBlockcode   {$$ = (AstNode *) newAstGraphNodeForeachNode((AstBlockcodeNode *) $7);free($4);}
;
nodeBlockcode:          '{' { pushScope(scopeTable); } nodeCode '}'                {$$ = (AstNode *) newAstBlockcodeNode((AstCodeNode *)$3);popScope(scopeTable);}
;
nodeCode:               ';'                             {$$ = (AstNode *) newAstCodeNode((AstNode *)NULL,(AstCodeNode *)NULL);}
                        |
                        nodeCode declaration ';'        {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        nodeCode conditional            {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        nodeCode forLoop                {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        nodeCode definition ';'         {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        nodeCode outputNode ';'             {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        nodeCode foreachEdge            {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        nodeCode nodeAction ';'         {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        nodeCode adjacencyDeclaration ';'  {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        nodeCode removeAdjacency ';'    {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        nodeCode traverseGraphNode ';'  {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                        |
                        code COMMENT
                        |
                                                        {$$ = (AstNode *) NULL;}
;
graphAction:            ID nodeProperty                 {
                                                            Symbol * symbol;
                                                            if((symbol = findSymbol(scopeTable,$1)) == NULL){
                                                                yyerror("Syntax Error: Usage of undeclared node.");
                                                            }
                                                            if(symbol->dataType != NODE_DECLARATION_TYPE) {
                                                                yyerror("Syntax Error: Cannot iterate over non Node data type.");
                                                            }
                                                            $$ = (AstNode *) newAstGraphActionNode($1,(AstIdNode *)$2);free($1);
                                                        }
;
nodeAction:     nodeField nodeProperty          {$$ = (AstNode *) newAstGraphActionNode($1,(AstIdNode *)$2);free($1);}
;
nodeProperty:   NODE_VALUE                      {$$ = (AstNode *) newAstIdNode("value",STRING_DECLARATION_TYPE);}
                |
                NODE_ID                         {$$ = (AstNode *) newAstIdNode("id",INT_DECLARATION_TYPE);}
;   
foreachEdge:    FOREACH '(' EDGE_ITERATOR IN nodeField ')' edgeBlockcode   {
                                                                                $$ = (AstNode *) newAstGraphEdgeForeachNode((AstBlockcodeNode *)$7, $5);
                                                                                free($4);
                                                                                free($5);
                                                                            }
;
nodeField:      ID                                                      {
                                                                            Symbol * symbol;
                                                                            if((symbol = findSymbol(scopeTable,$1)) == NULL){
                                                                                yyerror("Syntax Error: Usage of undeclared node.");
                                                                            }
                                                                            if(symbol->dataType != NODE_DECLARATION_TYPE) {
                                                                                yyerror("Syntax Error: Cannot iterate over non Node data type.");
                                                                            }
                                                                            $$ = $1;
                                                                        }
                |
                NODE_ITERATOR                                           { $$ = strdup("nodeIterator");}
;
edgeBlockcode:      '{' { pushScope(scopeTable); } edgeCode '}'                {$$ = (AstNode *) newAstBlockcodeNode((AstCodeNode *)$3);popScope(scopeTable);}
;
edgeCode:       ';'                             {$$ = (AstNode *) newAstCodeNode((AstNode *)NULL,(AstCodeNode *)NULL);}
                |
                edgeCode declaration ';'        {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                edgeCode conditional            {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                edgeCode forLoop                {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                edgeCode definition ';'         {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                edgeCode outputEdge ';'             {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                edgeCode nodeAction ';'         {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                edgeCode edgeAction ';'         {$$ = (AstNode *) newAstCodeNode($2,(AstCodeNode *)$1);}
                |
                code COMMENT
                |
                                                {$$ = (AstNode *) NULL;}
;
edgeAction:     EDGE_ITERATOR EDGE_WEIGHT       {$$ = (AstNode *) newAstGraphActionNode("edgeIterator",newAstIdNode("weight",EDGE_DECLARATION_TYPE));}
;    
definition:     ID '=' exp                  {
                                                Symbol * symbol;
                                                if((symbol=findSymbol(scopeTable,$1)) != NULL){
                                                    if(symbol->dataType != INT_DECLARATION_TYPE)
                                                        yyerror("Syntax Error: Invalid definition, expected int data type.");
                                                }else
                                                    yyerror("Syntax Error: Usage of undeclared node.");
                                                $$ = (AstNode *) newAstDefinitionNode($3,$1,INT_DECLARATION_TYPE);
                                                free($1);
                                            }
                |
                ID '=' STRING_VALUE         {
                                                Symbol * symbol;
                                                if((symbol=findSymbol(scopeTable,$1)) != NULL){
                                                    if(symbol->dataType == INT_DECLARATION_TYPE || symbol->dataType == NODE_DECLARATION_TYPE)
                                                        yyerror("Syntax Error: Invalid definition, expected int data type.");
                                                }else
                                                    yyerror("Syntax Error: Usage of undeclared variable.");
                                                $$ = (AstNode *) newAstDefinitionNode((AstNode *)newAstConstantExpressionNode($3),$1,STRING_DECLARATION_TYPE);
                                                free($1);
                                            }
                |
                ID '=' INPUT'(' ')'         {
                                                Symbol * symbol;
                                                if((symbol=findSymbol(scopeTable,$1)) != NULL){
                                                    if(symbol->dataType == INT_DECLARATION_TYPE || symbol->dataType == NODE_DECLARATION_TYPE )
                                                        yyerror("Syntax Error: Invalid definition data type, expected string data type.");
                                                }else
                                                    yyerror("Syntax Error: Usage of undeclared variable.");
                                                $$ = (AstNode *) newAstDefinitionNode((AstNode *)NULL,$1,INPUT_DECLARATION_TYPE);
                                                free($1);
                                            }
;
exp:            term                        {$$ = (AstNode *) newAstArithmeticExpressionNode((AstArithmeticExpressionNode *) NULL,(AstArithmeticExpressionNode *) NULL, (char *) NULL, $1);}
                |
                exp OPERATOR exp            {$$ = (AstNode *) newAstArithmeticExpressionNode((AstArithmeticExpressionNode *) $1, (AstArithmeticExpressionNode *) $3, $2, 0);free($2);}
;
term:           NUMBER                      {$$ = (AstNode *)newAstNumericExpressionNode($1);}
                |
                ID                          {
                                                Symbol * symbol;
                                                if((symbol=findSymbol(scopeTable,$1)) == NULL)
                                                    yyerror("Syntax Error: Usage of undeclared variable.");
                                                if(symbol->dataType != INT_DECLARATION_TYPE) 
                                                    yyerror("Syntax Error: Invalid definition data type, expected int data type.");
                                                $$ = (AstNode *)newAstIdNode($1, symbol->dataType);
                                                free($1);
                                            }
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
output:         OUTPUT '(' STRING_VALUE ')'       {$$ = (AstNode *) newAstPrintNode((AstNode *)newAstConstantExpressionNode($3));free($3);}
                |
                OUTPUT '(' ID ')'               {
                                                    Symbol * symbol;
                                                    if((symbol=findSymbol(scopeTable,$3)) == NULL)
                                                        yyerror("Syntax Error: Usage of undeclared variable.");
                                                    AstNode * node = (AstNode *) newAstIdNode($3,symbol->dataType);
                                                    $$ = (AstNode *) newAstPrintNode(node);
                                                    free($3);
                                                }
                |
                 OUTPUT '(' NUMBER ')'         {
                                                    AstNode * node = (AstNode*) newAstNumericExpressionNode($3);
                                                    $$ = (AstNode *) newAstPrintNode(node);
                                                }
                |
                 OUTPUT '(' graphAction ')'     {
                                                    $$ = (AstNode *) newAstPrintNode((AstNode*)$3);
                                                }
;   
outputNode:     OUTPUT '(' STRING_VALUE ')'       {$$ = (AstNode *) newAstPrintNode((AstNode *)newAstConstantExpressionNode($3));free($3);}
                |
                OUTPUT '(' ID ')'               {
                                                    Symbol * symbol;
                                                    if((symbol=findSymbol(scopeTable,$3)) == NULL)
                                                        yyerror("Syntax Error: Usage of undeclared variable.");
                                                    AstNode * node = (AstNode *) newAstIdNode($3,symbol->dataType);
                                                    $$ = (AstNode *) newAstPrintNode(node);
                                                    free($3);
                                                }
                |
                 OUTPUT '(' NUMBER ')'         {
                                                    AstNode * node = (AstNode*) newAstNumericExpressionNode($3);
                                                    $$ = (AstNode *) newAstPrintNode(node);
                                                }
                |
                 OUTPUT '(' nodeAction ')'     {
                                                    $$ = (AstNode *) newAstPrintNode((AstNode*)$3);
                                                }  
;
outputEdge:     OUTPUT '(' STRING_VALUE ')'       {$$ = (AstNode *) newAstPrintNode((AstNode *)newAstConstantExpressionNode($3));free($3);}
                |
                OUTPUT '(' ID ')'               {
                                                    Symbol * symbol;
                                                    if((symbol=findSymbol(scopeTable,$3)) == NULL)
                                                        yyerror("Syntax Error: Usage of undeclared variable.");
                                                    AstNode * node = (AstNode *) newAstIdNode($3,symbol->dataType);
                                                    $$ = (AstNode *) newAstPrintNode(node);
                                                    free($3);
                                                }
                |
                 OUTPUT '(' NUMBER ')'         {
                                                    AstNode * node = (AstNode*) newAstNumericExpressionNode($3);
                                                    $$ = (AstNode *) newAstPrintNode(node);
                                                }
                |
                 OUTPUT '(' nodeAction ')'     {
                                                    $$ = (AstNode *) newAstPrintNode((AstNode*)$3);
                                                }
                |
                 OUTPUT '(' EDGE_ITERATOR edgeProperty  ')'     {
                                                    $$ = (AstNode *) newAstPrintNode((AstNode*)newAstGraphActionNode("edgeIterator",newAstIdNode("weight",EDGE_DECLARATION_TYPE)));
                                                }   
;
edgeProperty:   EDGE_WEIGHT                    
                |
                                            
;                                        
forLoop:        FOR '(' {pushScope(scopeTable);} declaration ';'  boolExp ';' definition ')' forBlockcode {$$ = (AstNode *)newAstForNode((AstDeclarationNode *)$4,(AstBooleanExpressionNode *)$6,(AstDefinitionNode *)$8,(AstBlockcodeNode *)$10);}
;
forBlockcode:      '{' code '}'                {$$ = (AstNode *) newAstBlockcodeNode((AstCodeNode *)$2);popScope(scopeTable);}
;
traverseGraphNode:  TRAVERSE '(' GRAPH WITH traverseProcedure STARTING nodeField ')'   {
                                                                                        $$ = (AstNode *) newAstTraverseNode($7, (AstTraverseProcedureNode *) $5);
                                                                                        free($7);
                                                                                    }
;
traverseGraph:  TRAVERSE '(' GRAPH WITH traverseProcedure STARTING ID ')'             {
                                                                                        Symbol * symbol;
                                                                                        if((symbol = findSymbol(scopeTable,$7))==NULL){
                                                                                            yyerror("Syntax Error: Cannot iterate over undeclared node.");
                                                                                        }else if(symbol->dataType != NODE_DECLARATION_TYPE)
                                                                                            yyerror("Syntax Error: Cannot iterate over non Node data type."); 
                                                                                        $$ = (AstNode *) newAstTraverseNode($7, (AstTraverseProcedureNode *) $5);
                                                                                        free($7); 
                                                                                    }
;
traverseProcedure:  BFS                                                             { $$ = (AstNode*) newAstTraverseProcedureNode(BFS_PROCEDURE); }
                    |
                    DFS                                                             { $$ = (AstNode*) newAstTraverseProcedureNode(DFS_PROCEDURE); }
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
    fprintf (stderr, "at line %d: %s\n", yylineno,s);
    yylex_destroy();
    free(scopeTable);
    exit(EXIT_FAILURE);
} 