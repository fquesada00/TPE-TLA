#include <stdio.h>
#include "AST.h"
#include <stdlib.h>
#include <string.h>

AstGraphNode *newAstGraphNode(AstBlockcodeNode *node)
{
    printf(" at %s\n", __func__);
    AstGraphNode *graph = malloc(sizeof(AstGraphNode));
    graph->type = GRAPH_TYPE;
    graph->blockcode = node;
    return graph;
}
AstBlockcodeNode *newAstBlockcodeNode(AstCodeNode *node)
{
    printf(" at %s\n", __func__);
    AstBlockcodeNode *blockcode = malloc(sizeof(AstBlockcodeNode));
    blockcode->type = BLOCKCODE_TYPE;
    blockcode->code = node;
    return blockcode;
}

AstCodeNode *newAstCodeNode(AstNode *current, AstCodeNode *code)
{
    printf(" at %s\n", __func__);
    if (code == NULL)
    {
        code = malloc(sizeof(AstCodeNode));
    }
    code->type = CODE_TYPE;
    AstNodeList * node = newAstNodeList(current, code);
    if(code->statements == NULL) code->statements = node;
    code->lastStatement = node;
    return code;
}

AstNodeList *newAstNodeList(AstNode *current, AstCodeNode *code)
{
    printf(" at %s\n", __func__);
    AstNodeList *list = malloc(sizeof(AstNodeList));
    list->type = NODE_LIST_TYPE;
    list->current = current;
    if(code->lastStatement != NULL) code->lastStatement->next = list;
    else list->next = NULL;
    return list;
}

AstDeclarationNode *newAstDeclarationNode(AstDeclarationType data_type, AstArithmeticExpressionNode *node, char *name)
{
    printf(" at %s\n", __func__);
    AstDeclarationNode *declaration = malloc(sizeof(AstDeclarationNode));
    declaration->type = DECLARATION_TYPE;
    declaration->data_type = data_type;
    declaration->exp = node;
    declaration->name = malloc(sizeof(char) * strlen(name));
    strcpy(declaration->name, name);
    return declaration;
}

AstConstantExpressionNode *newAstConstantExpressionNode(char *stringValue)
{
    AstConstantExpressionNode *constantExpression = malloc(sizeof(AstConstantExpressionNode));
    constantExpression->type = STRING_DECLARATION_TYPE;
    int i = strlen(stringValue);
    constantExpression->stringValue = malloc(i * (sizeof(char)) + 1);
    strncpy(constantExpression->stringValue, stringValue, i);
    constantExpression->stringValue[i + 1] = 0;
    return constantExpression;
}

AstArithmeticExpressionNode *newAstArithmeticExpressionNode(AstArithmeticExpressionNode *right, AstArithmeticExpressionNode *left, char *op, int value)
{
    printf(" at %s\n", __func__);
    AstArithmeticExpressionNode *arithmeticExpression = malloc(sizeof(AstArithmeticExpressionNode));
    arithmeticExpression->type = ARITHMETIC_EXP_TYPE;
    if (right == NULL && left == NULL)
    {
        arithmeticExpression->value = value;
    }
    else
    {
        arithmeticExpression->op = malloc(sizeof(char) * strlen(op));
        strcpy(arithmeticExpression->op, op);
    }
    arithmeticExpression->right = right;
    arithmeticExpression->left = left;
    return arithmeticExpression;
}
