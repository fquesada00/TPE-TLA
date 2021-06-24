#include <stdio.h>
#include "AST.h"
#include <stdlib.h>
#include <string.h>
AstGraphNode * newAstGraphNode(AstBlockcodeNode * node){
    printf(" at %s\n", __func__);
    AstGraphNode * graph = malloc(sizeof(AstGraphNode));
    graph->type=GRAPH_TYPE;
    graph->blockcode=node;
    return graph; 
}
AstBlockcodeNode * newAstBlockcodeNode(AstCodeNode * node){
    printf(" at %s\n", __func__);
    AstBlockcodeNode * blockcode = malloc(sizeof(AstBlockcodeNode));
    blockcode->type=BLOCKCODE_TYPE;
    blockcode->code=node;
    return blockcode;
}

AstCodeNode * newAstCodeNode(AstNode * current, AstNodeList * next){
    printf(" at %s\n", __func__);
    AstCodeNode * code = malloc(sizeof(AstCodeNode));
    code->type =CODE_TYPE;
    code->statements = newAstNodeList(current, next);
    return code;
}

AstNodeList * newAstNodeList(AstNode * current,AstNodeList * next){
    printf(" at %s\n", __func__);
    AstNodeList * list = malloc(sizeof(AstNodeList));
    list->type=NODE_LIST_TYPE;
    list->current = current;
    list->next = next;
}

AstDeclarationNode * newAstDeclarationNode(AstDeclarationType data_type, AstArithmeticExpressionNode * node, char * name) {
    printf(" at %s\n", __func__);
    AstDeclarationNode * declaration = malloc(sizeof(AstDeclarationNode));
    declaration->type = DECLARATION_TYPE;
    declaration->data_type = data_type;
    declaration->exp = node;
    declaration->name = malloc(sizeof(char) * strlen(name));
    strcpy(declaration->name, name);
    return declaration;
}

AstArithmeticExpressionNode * newAstArithmeticExpressionNode(AstArithmeticExpressionNode * right, AstArithmeticExpressionNode * left, char * op, int value) {
    printf(" at %s\n", __func__);
    AstArithmeticExpressionNode * arithmeticExpression = malloc(sizeof(AstArithmeticExpressionNode));
    arithmeticExpression->type = ARITHMETIC_EXP_TYPE;
    if(right == NULL && left == NULL) {
        arithmeticExpression->value = value;
    } else {
        arithmeticExpression->op = malloc(sizeof(char) * strlen(op));
        strcpy(arithmeticExpression->op,op);
    }
    arithmeticExpression->right = right;
    arithmeticExpression->left = left;
}
    