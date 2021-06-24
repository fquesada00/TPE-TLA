#ifndef AST_H
#define AST_H

typedef enum {
    GRAPH_TYPE,
    BLOCKCODE_TYPE,
    CODE_TYPE,
    IF_TYPE,   
    DECLARATION_TYPE,
    NODE_LIST_TYPE,
    ARITHMETIC_EXP_TYPE
} AstNodeType;

typedef struct AstNode {
    AstNodeType type;
} AstNode;

typedef struct AstNodeList{
    AstNodeType type;
    AstNode * current;
    struct AstNodeList * next;
}AstNodeList;

typedef struct AstCodeNode{
    AstNodeType type;
    AstNodeList * statements;
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

typedef struct AstDeclarationNode {
    AstNodeType type;
    AstDeclarationType data_type;
    char * name;
    AstArithmeticExpressionNode * exp;
    // AstConstantExpressionNode * str;
} AstDeclarationNode;

AstGraphNode * newAstGraphNode(AstBlockcodeNode * node);
AstBlockcodeNode * newAstBlockcodeNode(AstCodeNode * node);
AstCodeNode * newAstCodeNode(AstNode * current,AstNodeList * next);
AstNodeList * newAstNodeList(AstNode * current,AstNodeList * next);
AstDeclarationNode * newAstDeclarationNode(AstDeclarationType data_type, AstArithmeticExpressionNode * node, char * name);
AstArithmeticExpressionNode * newAstArithmeticExpressionNode(AstArithmeticExpressionNode * right, AstArithmeticExpressionNode * left, char * op, int value);
#endif