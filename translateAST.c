#include "translateAST.h"
#include "AST.h"
#include <stdio.h>
#include <stdlib.h>

void translateMainBlockcode(AstBlockcodeNode *node)
{
    printf("{\n");
    printf("Graph * graph = newGraph();\n");
    printf("GraphNode * nodeIterator;\n");
    printf("GraphEdge * edgeIterator;\n");
    translateAstCodeNode(node->code);
    printf("}");
}

void translateAstGraphNode(AstGraphNode *node)
{
    printf("int main()");
    translateMainBlockcode(node->blockcode);
    freeAstGraphNode(node);
}

void translateAstBlockcodeNode(AstBlockcodeNode *node)
{
    printf("{\n");
    switch(node->type){
        case FOREACH_NODE_BLOCKCODE_TYPE:
        printf("nodeIterator = nextForEachNode(graph);\n");
        default:
        break;
    }
    translateAstCodeNode(node->code);
    printf("}");
}
void translateAstCodeNode(AstCodeNode *node)
{
    if (node != NULL)
    {
        translateAstNodeList(node->statements);
    }
}

void translateAstForNode(AstForNode *node)
{
    printf("for(");
    translateAstDeclarationNode(node->initialDeclaration);
    translateAstBooleanExpressionNode(node->loopCondition);
    printf(";");
    translateAstForDefinitionNode(node->reDeclaration);
    printf(")");
    translateAstBlockcodeNode(node->blockcode);
}

void translateAstIfNode(AstIfNode *node)
{
    if (node->type == IF_TYPE)
    {
        printf("if(");
        translateAstBooleanExpressionNode(node->condition);
        printf(")");
    }
    else
    {
        if (node->condition == NULL)
        {
            printf("else ");
        }
        else
        {
            printf("else if(");
            translateAstBooleanExpressionNode(node->condition);
            printf(")");
        }
    }
    translateAstBlockcodeNode(node->blockcode);
    if (node->next != NULL)
        translateAstIfNode(node->next);
}

void translateAstNodeList(AstNodeList *node)
{
    AstNodeList *it = node;
    while (it != NULL)
    {
        switch (it->current->type)
        {
        case CODE_TYPE:
            break;
        case ELSE_IF_TYPE:
        case IF_TYPE:
            translateAstIfNode((AstIfNode *)it->current);
            break;
        case FOR_TYPE:
            translateAstForNode((AstForNode *)it->current);
            break;
        case DECLARATION_TYPE:
            translateAstDeclarationNode((AstDeclarationNode *)it->current);
            break;
        case OUTPUT_TYPE:
            translateAstPrintNode((AstPrintNode *)it->current);
            break;
        case DEFINITION_TYPE:
            translateAstDefinitionNode((AstDefinitionNode *)it->current);
            break;
        case GRAPH_NODE_DECLARATION_TYPE:
            translateAstGraphNodeDeclarationNode((AstGraphNodeDeclarationNode *)it->current);
            break;
        case GRAPH_EDGE_DECLARATION_TYPE:
            translateAstGraphEdgeDeclarationNode((AstGraphEdgeDeclarationNode * ) it->current);
            break;
        case FOREACH_NODE_DECLARATION_TYPE:
            translateAstGraphNodeForeachNode((AstGraphNodeForeachNode *) it->current);
            break;
        case FOREACH_EDGE_DECLARATION_TYPE:
            translateAstGraphEdgeForeachNode((AstGraphEdgeForeachNode *) it->current);
            break;
        case GRAPH_NODE_ACTION_TYPE:
            translateAstGraphActionNode((AstGraphActionNode *) it->current);
            break;
        }
        printf("\n");
        it = it->next;
    }
}

void translateAstDeclarationNode(AstDeclarationNode *node)
{
    switch (node->dataType)
    {
    case INT_DECLARATION_TYPE:
        printf("int %s", node->name);
        if (node->exp != NULL)
        {
            printf(" = ");
            translateAstArithmeticExpressionNode(node->exp);
        }
        break;
    case STRING_DECLARATION_TYPE:
        printf("char * %s", node->name);
        if (node->str != NULL)
        {
            printf(" = ");
            translateAstConstantNode((AstConstantExpressionNode *)node->str);
        }
        break;
    case INPUT_DECLARATION_TYPE:
        printf("char * %s = ", node->name);
        translateInput(node->name);
    }
    printf(";");
}

void translateAstForDefinitionNode(AstDefinitionNode *node)
{
    printf("%s = ", node->name);
    switch (node->dataType)
    {
    case INT_DECLARATION_TYPE:
        translateAstArithmeticExpressionNode((AstArithmeticExpressionNode *)node->exp);
        break;
    case STRING_DECLARATION_TYPE:
        translateAstConstantNode((AstConstantExpressionNode *)node->str);
        break;
    case INPUT_DECLARATION_TYPE:
        translateInput(node->name);
        break;
    default:
        break;
    }
}

void translateAstDefinitionNode(AstDefinitionNode *node)
{
    translateAstForDefinitionNode(node);
    printf(";");
}

void translateInput(char *name)
{
    printf("malloc(MAX_INPUT_SIZE*sizeof(char));\n");
    printf("fgets(%s, MAX_INPUT_SIZE, stdin)", name);
}

void translateAstConstantNode(AstConstantExpressionNode *node)
{
    printf("%s", node->stringValue);
}

void translateAstNumericExpressionNode(AstNumericExpressionNode *node)
{
    printf("%d", node->value);
}

void translateAstIdNode(AstIdNode *node)
{
    printf("%s", node->name);
}

void translateAstArithmeticExpressionNode(AstArithmeticExpressionNode *node)
{
    if (node->left == NULL && node->right == NULL)
    {
        switch (node->value->type)
        {
        case NUMERIC_TYPE:
            translateAstNumericExpressionNode((AstNumericExpressionNode *)node->value);
            break;
        case ID_TYPE:
            translateAstIdNode((AstIdNode *)node->value);
            break;
        default:
            break;
        }
        return;
    }
    translateAstArithmeticExpressionNode(node->left);
    printf(" %s ", node->op);
    translateAstArithmeticExpressionNode(node->right);
}

void translateAstBooleanExpressionNode(AstBooleanExpressionNode *node)
{
    if (node->left == NULL && node->right == NULL)
    {
        switch (node->value->type)
        {
        case NUMERIC_TYPE:
            translateAstNumericExpressionNode((AstNumericExpressionNode *)node->value);
            break;
        case ID_TYPE:
            translateAstIdNode((AstIdNode *)node->value);
            break;
        default:
            break;
        }
        return;
    }
    else if (node->left == NULL)
    {
        printf("%s", node->op);
        translateAstBooleanExpressionNode(node->right);
        return;
    }
    else if (node->right == NULL)
    {
        printf("%s", node->op);
        translateAstBooleanExpressionNode(node->left);
        return;
    }

    translateAstBooleanExpressionNode(node->left);
    printf(" %s ", node->op);
    translateAstBooleanExpressionNode(node->right);
}

void translateAstPrintNode(AstPrintNode *node)
{
    printf("printf(");
    AstIdNode *n;
    switch (node->node->type)
    {
    case NUMERIC_TYPE:
        printf("\"%%d\",");
        printf("%d", ((AstNumericExpressionNode *)node->node)->value);
        break;
    case ID_TYPE:
        n = (AstIdNode *)node->node;
        switch (n->declarationType)
        {
        case INT_DECLARATION_TYPE:
            printf("\"%%d\",");
            break;
        case INPUT_DECLARATION_TYPE:
        case STRING_DECLARATION_TYPE:
            printf("\"%%s\",");
            break;
        default:
            break;
        }
        printf("%s", n->name);
        break;
    case CONSTANT_STRING_TYPE:
        printf("%s", ((AstConstantExpressionNode *)node->node)->stringValue);
    default:
        break;
    }
    printf(");");
}

void translateAstGraphNodeDeclarationNode(AstGraphNodeDeclarationNode *node)
{
    char *value;
    switch (node->value->type)
    {
    case CONSTANT_STRING_TYPE:
        value = ((AstConstantExpressionNode *)node->value)->stringValue;
        break;
    case ID_TYPE:
        value = ((AstIdNode *)node->value)->name;
        break;
    }
    printf("GraphNode * %s = newGraphNode(graph,%s);\n", node->name, value);
    if (node->next != NULL)
    {
        translateAstGraphNodeDeclarationNode(node->next);
    }
}

void translateAstGraphEdgeDeclarationNode(AstGraphEdgeDeclarationNode *node)
{
    char *value;
    AstIdNode * n;
    printf("addEdge(%s,%s,(void*)",node->leftNode,node->rightNode);
    switch (node->value->type)
    {
    case CONSTANT_STRING_TYPE:
        printf("%s,STRING_EDGE", ((AstConstantExpressionNode *)node->value)->stringValue);
        break;
    case ID_TYPE:
        n = (AstIdNode *) node->value;
        switch (n->declarationType)
        {
            case INT_DECLARATION_TYPE:
                printf("&%s,", ((AstIdNode *)node->value)->name);
                printf("INT_EDGE");
                break;
            case INPUT_DECLARATION_TYPE:
            case STRING_DECLARATION_TYPE:
                printf("%s,", ((AstIdNode *)node->value)->name);
                printf("STRING_EDGE");
                break;
            default:
                break;
        }
        break;
    case NUMERIC_TYPE:
        printf("(&(int){%d}),INT_EDGE", ((AstNumericExpressionNode *)node->value)->value);
        break;
    }
    printf(");");
}

void translateAstGraphNodeForeachNode(AstGraphNodeForeachNode * node){
    printf( "beginForEachNode(graph);\n"
            "while (hasNextForEachNode(graph))");
    translateAstBlockcodeNode(node->blockcode);
}
void translateAstGraphEdgeForeachNode(AstGraphEdgeForeachNode *node){
    char * name = node->nodeName;
    printf( "beginForEachEdge(%s);\n"
            "while(hasNextForEachEdge(%s)){\n"
            "edgeIterator = nextForEachEdge(%s);\n", name, name, name);
    translateAstCodeNode(node->blockcode->code);
    printf("}");
}

void translateAstGraphActionNode(AstGraphActionNode *node){
    printf("%s->%s;",node->nodeName, node->property->name);
}