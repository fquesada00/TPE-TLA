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
    AstNodeList *node = newAstNodeList(current, code);
    if (code->statements == NULL)
        code->statements = node;
    code->lastStatement = node;
    return code;
}

AstNodeList *newAstNodeList(AstNode *current, AstCodeNode *code)
{
    // printf(" at %s\n", __func__);
    AstNodeList *list = malloc(sizeof(AstNodeList));
    list->type = NODE_LIST_TYPE;
    list->current = current;
    if (code != NULL && code->lastStatement != NULL)
        code->lastStatement->next = list;
    list->next = NULL;
    return list;
}

AstDeclarationNode *newAstDeclarationNode(AstNode *node, char *name, AstDeclarationType dataType)
{
    // printf(" at %s\n", __func__);
    AstDeclarationNode *declaration = malloc(sizeof(AstDeclarationNode));
    declaration->type = DECLARATION_TYPE;
    declaration->dataType = dataType;
    switch (dataType)
    {
    case INT_DECLARATION_TYPE:
        declaration->exp = (AstArithmeticExpressionNode *)node;
        break;
    case INPUT_DECLARATION_TYPE:
    case STRING_DECLARATION_TYPE:
        declaration->str = (AstConstantExpressionNode *)node;
        break;
    }
    declaration->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(declaration->name, name);
    return declaration;
}

AstPrintNode *newAstPrintNode(AstNode *node)
{
    AstPrintNode *new = malloc(sizeof(AstPrintNode));
    new->type = OUTPUT_TYPE;
    new->node = node;
    return new;
}

AstDefinitionNode *newAstDefinitionNode(AstNode *node, char *name, AstDeclarationType dataType)
{
    AstDefinitionNode *definition = malloc(sizeof(AstDefinitionNode));
    definition->type = DEFINITION_TYPE;
    definition->dataType = dataType;
    switch (dataType)
    {
    case INT_DECLARATION_TYPE:
        definition->exp = (AstArithmeticExpressionNode *)node;
        break;
    case INPUT_DECLARATION_TYPE:
    case STRING_DECLARATION_TYPE:
        definition->str = (AstConstantExpressionNode *)node;
        break;
    }
    definition->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(definition->name, name);
    return definition;
}

AstIfNode *newAstIfNode(AstBooleanExpressionNode *condition, AstBlockcodeNode *blockcode, int type, AstIfNode *next)
{
    AstIfNode *new = malloc(sizeof(AstIfNode));
    new->type = type;
    new->condition = condition;
    new->blockcode = blockcode;
    new->next = next;
    return new;
}

AstForNode *newAstForNode(AstDeclarationNode *initialDeclaration, AstBooleanExpressionNode *loopCondition, AstDefinitionNode *reDeclaration, AstBlockcodeNode *blockcode)
{
    AstForNode *new = malloc(sizeof(AstForNode));
    new->type = FOR_TYPE;
    new->initialDeclaration = initialDeclaration;
    new->loopCondition = loopCondition;
    new->reDeclaration = reDeclaration;
    new->blockcode = blockcode;
    return new;
}

AstConstantExpressionNode *newAstConstantExpressionNode(char *stringValue)
{
    AstConstantExpressionNode *constantExpression = malloc(sizeof(AstConstantExpressionNode));
    constantExpression->type = CONSTANT_STRING_TYPE;
    int i = strnlen(stringValue, MAX_STRING_LENGTH);
    constantExpression->stringValue = malloc((i + 1) * (sizeof(char)));
    strncpy(constantExpression->stringValue, stringValue, i);
    constantExpression->stringValue[i] = 0;
    return constantExpression;
}

AstBooleanExpressionNode *newAstBooleanExpressionNode(AstBooleanExpressionNode *left, AstBooleanExpressionNode *right, char *op, AstNode *value)
{
    AstBooleanExpressionNode *booleanExpression = malloc(sizeof(AstBooleanExpressionNode));
    booleanExpression->type = BOOLEAN_EXP_TYPE;
    if (left == NULL && right == NULL)
    {
        booleanExpression->value = value;
    }
    else
    {
        int length = strlen(op);
        booleanExpression->op = malloc(sizeof(char) * (length + 1));
        strncpy(booleanExpression->op, op, length);
        booleanExpression->op[length] = 0;
    }
    booleanExpression->left = left;
    booleanExpression->right = right;
    return booleanExpression;
}

AstArithmeticExpressionNode *newAstArithmeticExpressionNode(AstArithmeticExpressionNode *left, AstArithmeticExpressionNode *right, char *op, AstNode *value)
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
        strncpy(arithmeticExpression->op, op, length);
        arithmeticExpression->op[length] = 0;
    }
    arithmeticExpression->right = right;
    arithmeticExpression->left = left;
    return arithmeticExpression;
}

AstNumericExpressionNode *newAstNumericExpressionNode(int value)
{
    AstNumericExpressionNode *new = malloc(sizeof(AstNumericExpressionNode));
    new->type = NUMERIC_TYPE;
    new->value = value;
    return new;
}

AstIdNode *newAstIdNode(char *name, AstDeclarationType type)
{
    AstIdNode *new = malloc(sizeof(AstIdNode));
    new->type = ID_TYPE;
    int i = strnlen(name, MAX_STRING_LENGTH);
    new->name = malloc((i + 1) * sizeof(char));
    strncpy(new->name, name, i + 1);
    new->declarationType = type;
    return new;
}

AstGraphNodeDeclarationNode *newAstGraphNodeDeclarationNode(char *name, AstGraphNodeDeclarationNode *next, AstNode *node)
{
    AstGraphNodeDeclarationNode *new = malloc(sizeof(AstGraphNodeDeclarationNode));
    new->type = GRAPH_NODE_DECLARATION_TYPE;
    int len = strnlen(name, MAX_STRING_LENGTH);
    new->name = malloc((len + 1) * sizeof(char));
    strncpy(new->name, name, len + 1);
    new->next = next;
    new->value = node;
    return new;
}

AstGraphEdgeDeclarationNode *newAstGraphEdgeDeclarationNode(char *leftNode, char *rightNode, AstNode *value)
{
    AstGraphEdgeDeclarationNode *new = malloc(sizeof(AstGraphEdgeDeclarationNode));
    int len1 = strnlen(leftNode, MAX_STRING_LENGTH);
    int len2 = strnlen(rightNode, MAX_STRING_LENGTH);
    new->leftNode = malloc(sizeof(char) * (len1 + 1));
    new->rightNode = malloc(sizeof(char) * (len2 + 1));
    strncpy(new->leftNode, leftNode, len1 + 1);
    strncpy(new->rightNode, rightNode, len2 + 1);
    new->type=GRAPH_EDGE_DECLARATION_TYPE;
    new->value = value;
    return new;
}

AstGraphNodeForeachNode * newAstGraphNodeForeachNode(AstBlockcodeNode * blockcode) {
    AstGraphNodeForeachNode * new = malloc(sizeof(AstGraphNodeForeachNode));
    new->type = FOREACH_NODE_DECLARATION_TYPE;
    new->blockcode = blockcode;
    new->blockcode->type = FOREACH_NODE_BLOCKCODE_TYPE;
    return new;
}

AstGraphEdgeForeachNode * newAstGraphEdgeForeachNode(AstBlockcodeNode * blockcode,char * nodeName){
    AstGraphEdgeForeachNode * new = malloc(sizeof(AstGraphEdgeForeachNode));
    int len = strnlen(nodeName, MAX_STRING_LENGTH);
    new->type = FOREACH_EDGE_DECLARATION_TYPE;
    new->nodeName = malloc((len+1) * sizeof(char));
    strncpy(new->nodeName, nodeName,len + 1);
    new->blockcode = blockcode;
    return new;
}

AstGraphActionNode * newAstGraphActionNode(char * nodeName,AstIdNode * property){
    AstGraphActionNode * new = malloc(sizeof(AstGraphActionNode));
    new->type = GRAPH_NODE_ACTION_TYPE;
    int len = strnlen(nodeName, MAX_STRING_LENGTH);
    new->nodeName = malloc((len+1) * sizeof(char));
    strncpy(new->nodeName, nodeName,+len +1);
    new->property = property;
    return new;
}

AstEdgeRemoveNode * newAstEdgeRemoveNode(char * leftNode,char * rightNode){
    AstEdgeRemoveNode * new = malloc(sizeof(AstEdgeRemoveNode));
    new->type = GRAPH_EDGE_REMOVE_TYPE;
    int len1,len2;
    len1 = strnlen(leftNode, MAX_STRING_LENGTH);
    len2 = strnlen(rightNode,MAX_STRING_LENGTH);
    new->leftNode = malloc((len1+1) * sizeof(char));
    new->rightNode = malloc((len2+1) * sizeof(char));
    strncpy(new->leftNode, leftNode,len1+1);
    strncpy(new->rightNode, rightNode,len2 + 1);
    return new;
}
AstNodeRemoveNode * newAstNodeRemoveNode(char * nodeName){
    
}



void freeAstArithmeticExpressionNode(AstArithmeticExpressionNode *node)
{
    if (node->left != NULL && node->right != NULL)
    {
        freeAstArithmeticExpressionNode(node->right);
        free(node->op);
        freeAstArithmeticExpressionNode(node->left);
    }
    else
    {
        switch (node->value->type)
        {
        case NUMERIC_TYPE:
            freeAstNumericExpressionNode((AstNumericExpressionNode *)node->value);
            break;
        case ID_TYPE:
            freeAstIdNode((AstIdNode *)node->value);
        default:
            break;
        }
    }
    free(node);
}

void freeAstConstantExpressionNode(AstConstantExpressionNode *node)
{
    free(node->stringValue);
    free(node);
}

void freeAstBooleanExpressionNode(AstBooleanExpressionNode *node)
{
    if (node->left != NULL && node->right != NULL)
    {
        freeAstBooleanExpressionNode(node->right);
        free(node->op);
        freeAstBooleanExpressionNode(node->left);
    }
    else if (node->left == NULL && node->right != NULL)
    {
        freeAstBooleanExpressionNode(node->right);
        free(node->op);
    }
    else if (node->left != NULL && node->right == NULL)
    {
        freeAstBooleanExpressionNode(node->left);
        free(node->op);
    }
    else
    {
        switch (node->value->type)
        {
        case NUMERIC_TYPE:
            freeAstNumericExpressionNode((AstNumericExpressionNode *)node->value);
            break;
        case ID_TYPE:
            freeAstIdNode((AstIdNode *)node->value);
        default:
            break;
        }
    }
    free(node);
}

void freeAstDeclarationNode(AstDeclarationNode *node)
{
    switch (node->dataType)
    {
    case INT_DECLARATION_TYPE:
        if (node->exp != NULL)
            freeAstArithmeticExpressionNode(node->exp);
        break;
    case INPUT_DECLARATION_TYPE:
    case STRING_DECLARATION_TYPE:
        if (node->str != NULL)
            freeAstConstantExpressionNode(node->str);
        break;
    }
    free(node->name);
    free(node);
}

void freeAstDefinitionNode(AstDefinitionNode *node)
{
    switch (node->dataType)
    {
    case INT_DECLARATION_TYPE:
        if (node->exp != NULL)
            freeAstArithmeticExpressionNode(node->exp);
        break;
    case INPUT_DECLARATION_TYPE:
    case STRING_DECLARATION_TYPE:
        if (node->str != NULL)
            freeAstConstantExpressionNode(node->str);
        break;
    }
    free(node->name);
    free(node);
}

void freeAstNumericExpressionNode(AstNumericExpressionNode *node)
{
    free(node);
}

void freeAstNodeList(AstNodeList *node)
{
    AstNodeList *curr, *it = node;
    while (it != NULL)
    {
        if (it->current != NULL)
        {
            switch (it->current->type)
            {
            case DECLARATION_TYPE:
                freeAstDeclarationNode((AstDeclarationNode *)it->current);
                break;
            case ARITHMETIC_EXP_TYPE:
                freeAstArithmeticExpressionNode((AstArithmeticExpressionNode *)it->current);
                break;
            case IF_TYPE:
                freeAstIfNode((AstIfNode *)it->current);
                break;
            case DEFINITION_TYPE:
                freeAstDefinitionNode((AstDefinitionNode *)it->current);
                break;
            case FOR_TYPE:
                freeAstForNode((AstForNode *)it->current);
                break;
            case OUTPUT_TYPE:
                freeAstPrintNode((AstPrintNode *)it->current);
                break;
            case GRAPH_NODE_DECLARATION_TYPE:
                freeAstGraphNodeDeclarationNode((AstGraphNodeDeclarationNode *)it->current);
                break;
            case GRAPH_EDGE_DECLARATION_TYPE:
                freeAstGraphEdgeDeclarationNode((AstGraphEdgeDeclarationNode *) it->current);
                break;
            case FOREACH_NODE_DECLARATION_TYPE:
                freeAstGraphNodeForeachNode((AstGraphNodeForeachNode *) it->current);
                break;
            case FOREACH_EDGE_DECLARATION_TYPE:
                freeAstGraphEdgeForeachNode((AstGraphEdgeForeachNode *) it->current);
                break;
            case GRAPH_NODE_ACTION_TYPE:
                freeAstGraphActionNode((AstGraphActionNode *) it->current);
                break;
            case  GRAPH_EDGE_REMOVE_TYPE:
                freeAstEdgeRemoveNode((AstEdgeRemoveNode * ) it->current);
                break;
            case GRAPH_NODE_REMOVE_TYPE:
                freeAstNodeRemoveNode((AstNodeRemoveNode *) it->current);
                break;
            }
            curr = it;
            it = it->next;
            free(curr);
        }
    }
}

void freeAstCodeNode(AstCodeNode *node)
{
    if (node != NULL)
    {
        freeAstNodeList(node->statements);
        free(node);
    }
}

void freeAstForNode(AstForNode *node)
{
    freeAstDeclarationNode(node->initialDeclaration);
    freeAstBooleanExpressionNode(node->loopCondition);
    freeAstDefinitionNode(node->reDeclaration);
    freeAstBlockcodeNode(node->blockcode);
    free(node);
}

void freeAstBlockcodeNode(AstBlockcodeNode *node)
{
    freeAstCodeNode(node->code);
    free(node);
}
void freeAstIdNode(AstIdNode *node)
{
    free(node->name);
    free(node);
}

void freeAstIfNode(AstIfNode *node)
{
    if (node->condition != NULL)
        freeAstBooleanExpressionNode(node->condition);
    freeAstBlockcodeNode(node->blockcode);
    if (node->next != NULL)
        freeAstIfNode(node->next);
    free(node);
}

void freeAstGraphNode(AstGraphNode *node)
{
    freeAstBlockcodeNode(node->blockcode);
    free(node);
}

void freeAstPrintNode(AstPrintNode *node)
{
    switch (node->node->type)
    {
    case NUMERIC_TYPE:
        freeAstNumericExpressionNode((AstNumericExpressionNode *)node->node);
        break;
    case ID_TYPE:
        freeAstIdNode((AstIdNode *)node->node);
        break;
    case CONSTANT_STRING_TYPE:
        freeAstConstantExpressionNode((AstConstantExpressionNode *)node->node);
        break;
    default:
        break;
    }
    free(node);
}

void freeAstGraphNodeDeclarationNode(AstGraphNodeDeclarationNode *node)
{
    free(node->name);
    if (node->value != NULL)
    {
        switch (node->value->type)
        {
        case CONSTANT_STRING_TYPE:
            freeAstConstantExpressionNode((AstConstantExpressionNode *)node->value);
            break;
        case ID_TYPE:
            freeAstIdNode((AstIdNode *)node->value);
            break;
        default:
            break;
        }
    }
    else
    {
        perror("Somthing went wrong...");
    }
    if (node->next != NULL)
        freeAstGraphNodeDeclarationNode(node->next);
    free(node);
}

void freeAstGraphEdgeDeclarationNode(AstGraphEdgeDeclarationNode *node)
{
    free(node->leftNode);
    free(node->rightNode);
    switch (node->value->type)
    {
    case NUMERIC_TYPE:
        freeAstNumericExpressionNode((AstNumericExpressionNode *)node->value);
        break;
    case ID_TYPE:
        freeAstIdNode((AstIdNode *)node->value);
        break;
    case CONSTANT_STRING_TYPE:
        freeAstConstantExpressionNode((AstConstantExpressionNode *)node->value);
        break;
    default:
        break;
    }
    free(node);
}

void freeAstGraphNodeForeachNode(AstGraphNodeForeachNode *node){
    freeAstBlockcodeNode(node->blockcode);
    free(node);
}

void freeAstGraphEdgeForeachNode(AstGraphEdgeForeachNode * node){
    freeAstBlockcodeNode(node->blockcode);
    free(node->nodeName);
    free(node);
}

void freeAstGraphActionNode(AstGraphActionNode * node) {
    free(node->nodeName);
    freeAstIdNode(node->property);
    free(node);
}

void freeAstEdgeRemoveNode(AstEdgeRemoveNode * node){
    free(node->leftNode);
    free(node->rightNode);
    free(node);
}

void freeAstNodeRemoveNode(AstNodeRemoveNode * node){
    free(node->nodeName);
    free(node);    
}