#ifndef AST_H
#define AST_H
#define MAX_STRING_LENGTH 256

typedef enum {
    GRAPH_TYPE,
    BLOCKCODE_TYPE,
    CODE_TYPE,
    IF_TYPE,
    ELSE_IF_TYPE,   
    DECLARATION_TYPE,
    NODE_LIST_TYPE,
    ARITHMETIC_EXP_TYPE,
    BOOLEAN_EXP_TYPE,
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

typedef enum {
    INT_DECLARATION_TYPE,
    STRING_DECLARATION_TYPE
} AstDeclarationType;

typedef struct AstArithmeticExpressionNode{
    AstNodeType type;
    struct AstArithmeticExpressionNode * left;
    struct AstArithmeticExpressionNode * right;
    char * op;
    int value;
}AstArithmeticExpressionNode;

typedef struct AstBooleanExpressionNode{
    AstNodeType type;
    struct AstBooleanExpressionNode * left;
    struct AstBooleanExpressionNode * right;
    char * op;
    int value;
} AstBooleanExpressionNode;

typedef struct AstConstantExpressionNode {
    AstNodeType type;
    char * stringValue;
} AstConstantExpressionNode;

typedef struct AstDeclarationNode {
    AstNodeType type;
    AstDeclarationType data_type;
    char * name;
    AstArithmeticExpressionNode * exp;
    AstConstantExpressionNode * str;
} AstDeclarationNode;

AstGraphNode * newAstGraphNode(AstBlockcodeNode * node);
AstBlockcodeNode * newAstBlockcodeNode(AstCodeNode * node);
AstCodeNode *newAstCodeNode(AstNode *current, AstCodeNode *code);
AstNodeList * newAstNodeList(AstNode * current,AstCodeNode * next);
AstDeclarationNode * newAstDeclarationNode(AstDeclarationType data_type, AstNode * node, char * name);
AstBooleanExpressionNode *newAstBooleanExpressionNode(AstBooleanExpressionNode *left, AstBooleanExpressionNode *right, char *op ,int value);
AstIfNode * newAstIfNode(AstBooleanExpressionNode * condition,AstBlockcodeNode * blockcode,int type, AstIfNode * next);
AstArithmeticExpressionNode *newAstArithmeticExpressionNode(AstArithmeticExpressionNode *left, AstArithmeticExpressionNode *right, char *op, int value);
AstConstantExpressionNode * newAstConstantExpressionNode(char * stringValue);
void freeAstGraphNode(AstGraphNode * node);
#endif