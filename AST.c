#include <stdio.h>
#include "AST.h"
#include <stdlib.h>
#include <string.h>

AstGraphNode *newAstGraphNode(AstBlockcodeNode *node)
{
    // printf(" at %s\n", __func__);
    AstGraphNode *graph = malloc(sizeof(AstGraphNode));
    graph->type = GRAPH_TYPE;
    graph->blockcode = node;
    return graph;
}
AstBlockcodeNode *newAstBlockcodeNode(AstCodeNode *node)
{
    // printf(" at %s\n", __func__);
    AstBlockcodeNode *blockcode = malloc(sizeof(AstBlockcodeNode));
    blockcode->type = BLOCKCODE_TYPE;
    blockcode->code = node;
    return blockcode;
}

AstCodeNode *newAstCodeNode(AstNode *current, AstCodeNode *code)
{
    // printf(" at %s\n", __func__);
    if (code == NULL)
    {
        code = malloc(sizeof(AstCodeNode));
        code->type = CODE_TYPE;
        code->statements = NULL;
        code->lastStatement = NULL;
    }
    AstNodeList * node = newAstNodeList(current, code);
    if(code->statements == NULL) code->statements = node;
    code->lastStatement = node;
    return code;
}

AstNodeList *newAstNodeList(AstNode *current, AstCodeNode *code)
{
    // printf(" at %s\n", __func__);
    AstNodeList *list = malloc(sizeof(AstNodeList));
    list->type = NODE_LIST_TYPE;
    list->current = current;
    if(code != NULL && code->lastStatement != NULL) code->lastStatement->next = list;
    list->next = NULL;
    return list;
}

AstDeclarationNode *newAstDeclarationNode(AstDeclarationType data_type, AstNode *node, char *name)
{
    // printf(" at %s\n", __func__);
    AstDeclarationNode *declaration = malloc(sizeof(AstDeclarationNode));
    declaration->type = DECLARATION_TYPE;
    declaration->data_type = data_type;
    switch(data_type){
        case INT_DECLARATION_TYPE:
            declaration->exp = (AstArithmeticExpressionNode *)node;
            break;
        case STRING_DECLARATION_TYPE:
            declaration->str = (AstConstantExpressionNode *) node;
            break;
    }
    declaration->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(declaration->name, name);
    return declaration;
}

AstIfNode * newAstIfNode(AstBooleanExpressionNode * condition,AstBlockcodeNode * blockcode,int type, AstIfNode * next){
    AstIfNode * new = malloc(sizeof(AstIfNode));
    new->type = type;
    new->condition = condition;
    new->blockcode = blockcode;
    new->next = next;
    return new;
}

AstConstantExpressionNode *newAstConstantExpressionNode(char *stringValue)
{
    AstConstantExpressionNode *constantExpression = malloc(sizeof(AstConstantExpressionNode));
    constantExpression->type = STRING_DECLARATION_TYPE;
    int i = strnlen(stringValue,MAX_STRING_LENGTH);
    constantExpression->stringValue = malloc((i+1) * (sizeof(char)));
    strncpy(constantExpression->stringValue, stringValue, i);
    constantExpression->stringValue[i] = 0;
    return constantExpression;
}

AstBooleanExpressionNode *newAstBooleanExpressionNode(AstBooleanExpressionNode *left, AstBooleanExpressionNode *right, char *op ,int value){
    AstBooleanExpressionNode * booleanExpression = malloc(sizeof(AstBooleanExpressionNode));
    booleanExpression->type = BOOLEAN_EXP_TYPE;
    if(left == NULL && right == NULL){
        booleanExpression->value = value;
    } 
    else {
        int length = strlen(op);
        booleanExpression->op = malloc(sizeof(char) * (length + 1));
        strncpy(booleanExpression->op,op,length);
        booleanExpression->op[length] = 0;
    }
    booleanExpression->left = left;
    booleanExpression->right = right;
    return booleanExpression;
}

AstArithmeticExpressionNode *newAstArithmeticExpressionNode(AstArithmeticExpressionNode *left, AstArithmeticExpressionNode *right, char *op, int value)
{
    // printf(" at %s\n", __func__);
    AstArithmeticExpressionNode *arithmeticExpression = malloc(sizeof(AstArithmeticExpressionNode));
    arithmeticExpression->type = ARITHMETIC_EXP_TYPE;
    if (right == NULL && left == NULL)
    {
        arithmeticExpression->value = value;
    }
    else
    {
        int length = strlen(op);
        arithmeticExpression->op = malloc(sizeof(char) * (length + 1));
        strncpy(arithmeticExpression->op, op,length);
        arithmeticExpression->op[length] = 0;
    }
    arithmeticExpression->right = right;
    arithmeticExpression->left = left;
    return arithmeticExpression;
}

void freeAstArithmeticExpressionNode(AstArithmeticExpressionNode * node){
    if(node->left != NULL && node->right != NULL){
        freeAstArithmeticExpressionNode(node->right);
        free(node->op);
        freeAstArithmeticExpressionNode(node->left);
    }
    free(node);
}

void freeAstConstantExpressionNode(AstConstantExpressionNode * node){
    free(node->stringValue);
    free(node);
}

void freeAstIfNode(AstIfNode * node){
    //TODO
}

void freeAstDeclarationNode(AstDeclarationNode * node){
    switch(node->data_type){
        case INT_DECLARATION_TYPE:
            if(node->exp != NULL) freeAstArithmeticExpressionNode(node->exp);
            break;
        case STRING_DECLARATION_TYPE:
            if(node->str != NULL) freeAstConstantExpressionNode(node->str);
            break;
    }
    free(node->name);
    free(node);
}


void freeAstNodeList(AstNodeList * node){
    AstNodeList *curr,* it = node;
    while(it != NULL){
        if(it->current != NULL){
            switch(it->current->type){
                case DECLARATION_TYPE:
                    freeAstDeclarationNode((AstDeclarationNode *)it->current);
                    break;
                case ARITHMETIC_EXP_TYPE:
                    freeAstArithmeticExpressionNode((AstArithmeticExpressionNode *)it->current);
                    break;
                case IF_TYPE:
                    freeAstIfNode((AstIfNode *)it->current);
                    break;
            }
            curr = it;
            it = it->next;
            free(curr);
        }
    }
}

void freeAstCodeNode(AstCodeNode * node){
    if(node != NULL){
        freeAstNodeList(node->statements);
        free(node);
    }
}

void freeAstBlockcodeNode(AstBlockcodeNode * node){
    freeAstCodeNode(node->code);
    free(node);
}

void freeAstGraphNode(AstGraphNode * node){
    freeAstBlockcodeNode(node->blockcode);
    free(node);
}
