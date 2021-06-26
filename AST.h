#ifndef AST_H
#define AST_H
#define MAX_STRING_LENGTH 256

typedef enum {
    GRAPH_TYPE,
    BLOCKCODE_TYPE,
    CODE_TYPE,
    FOR_TYPE,
    IF_TYPE,
    ELSE_IF_TYPE,   
    DECLARATION_TYPE,
    CONSTANT_STRING_TYPE,
    NUMERIC_TYPE,
    DEFINITION_TYPE,
    NODE_LIST_TYPE,
    ARITHMETIC_EXP_TYPE,
    BOOLEAN_EXP_TYPE,
    INPUT_TYPE,
    OUTPUT_TYPE,
    ID_TYPE
} AstNodeType;

typedef struct AstNode {
    AstNodeType type;
} AstNode;

typedef struct AstNodeList{
    AstNodeType type;
    AstNode * current;
    struct AstNodeList * next;
}AstNodeList; //Este es el item de cada lista


typedef struct AstCodeNode{ //Esta es la wrapper de la lista.
    AstNodeType type;
    AstNodeList * statements; // int a=5;string hola;
    AstNodeList * lastStatement;
}AstCodeNode;

typedef struct AstBlockcodeNode{
    AstNodeType type;
    AstCodeNode * code;
}AstBlockcodeNode;

typedef struct AstGraphNode {
    AstNodeType type;
    AstBlockcodeNode * blockcode;
} AstGraphNode;

typedef struct AstIfNode {
    AstNodeType type;
    struct AstBooleanExpressionNode * condition;
    AstBlockcodeNode * blockcode;
    struct AstIfNode * next;
} AstIfNode;

typedef struct AstPrintNode {
    AstNodeType type;
    char * string;   
} AstPrintNode;

typedef struct AstGetNode {
    AstNodeType type;
    char * string;
}AstGetNode;

typedef struct AstForNode{
    AstNodeType type;
    struct AstDeclarationNode * initialDeclaration;
    struct AstBooleanExpressionNode * loopCondition;
    struct AstDefinitionNode * reDeclaration;
    struct AstBlockcodeNode * blockcode;
}AstForNode;

typedef enum {
    INT_DECLARATION_TYPE,
    STRING_DECLARATION_TYPE
} AstDeclarationType;

typedef struct AstIdNode{
    AstNodeType type;
    AstDeclarationType declarationType;
    char * name;
} AstIdNode;


typedef struct AstArithmeticExpressionNode{
    AstNodeType type;
    struct AstArithmeticExpressionNode * left;
    struct AstArithmeticExpressionNode * right;
    char * op;
    AstNode * value;
}AstArithmeticExpressionNode;

typedef struct AstBooleanExpressionNode{
    AstNodeType type;
    struct AstBooleanExpressionNode * left;
    struct AstBooleanExpressionNode * right;
    char * op;
    AstNode * value;
} AstBooleanExpressionNode;

typedef struct AstConstantExpressionNode {
    AstNodeType type;
    char * stringValue;
} AstConstantExpressionNode;

typedef struct AstNumericExpressionNode {
    AstNodeType type;
    int value;
} AstNumericExpressionNode;

typedef struct AstDeclarationNode {
    AstNodeType type;
    AstDeclarationType dataType;
    char * name;
    AstArithmeticExpressionNode * exp;
    AstConstantExpressionNode * str;
} AstDeclarationNode;

typedef struct AstDefinitionNode {
    AstNodeType type;
    char * name;
    AstDeclarationType dataType;
    AstArithmeticExpressionNode * exp;
    AstConstantExpressionNode * str;
}AstDefinitionNode;

AstGraphNode * newAstGraphNode(AstBlockcodeNode * node);
AstBlockcodeNode * newAstBlockcodeNode(AstCodeNode * node);
AstCodeNode *newAstCodeNode(AstNode *current, AstCodeNode *code);
AstNodeList * newAstNodeList(AstNode * current,AstCodeNode * next);
AstDeclarationNode * newAstDeclarationNode(AstNode * node, char * name,AstDeclarationType dataType);
AstBooleanExpressionNode *newAstBooleanExpressionNode(AstBooleanExpressionNode *left, AstBooleanExpressionNode *right, char *op ,AstNode * value);
AstIfNode * newAstIfNode(AstBooleanExpressionNode * condition,AstBlockcodeNode * blockcode,int type, AstIfNode * next);
AstForNode * newAstForNode(AstDeclarationNode * initialDeclaration,AstBooleanExpressionNode * loopCondition,AstDefinitionNode * definition,AstBlockcodeNode * blockcode);
AstArithmeticExpressionNode *newAstArithmeticExpressionNode(AstArithmeticExpressionNode *left, AstArithmeticExpressionNode *right, char *op, AstNode * value);
AstConstantExpressionNode * newAstConstantExpressionNode(char * stringValue);
AstDefinitionNode * newAstDefinitionNode(AstNode * node,char * name, AstDeclarationType dataType);
AstNumericExpressionNode * newAstNumericExpressionNode( int value);
AstIdNode * newAstIdNode(char * name,AstDeclarationType type);
void freeAstArithmeticExpressionNode(AstArithmeticExpressionNode * node);
void freeAstConstantExpressionNode(AstConstantExpressionNode * node);
void freeAstBooleanExpressionNode(AstBooleanExpressionNode * node);
void freeAstDeclarationNode(AstDeclarationNode * node);
void freeAstDefinitionNode(AstDefinitionNode * node);
void freeAstGraphNode(AstGraphNode * node);
void freeAstNodeList(AstNodeList * node);
void freeAstIfNode(AstIfNode * node);
void freeAstBlockcodeNode(AstBlockcodeNode * node);
void freeAstCodeNode(AstCodeNode * node);
void freeAstForNode(AstForNode * node);
void freeAstNumericExpressionNode(AstNumericExpressionNode *node);
void freeAstIdNode(AstIdNode * node);
#endif