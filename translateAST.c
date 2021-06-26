#include "translateAST.h"
#include "AST.h"
#include <stdio.h>
#include <stdlib.h>

void translateAstGraphNode(AstGraphNode * node){
    printf("int main()");
    translateAstBlockcodeNode(node->blockcode);
    freeAstGraphNode(node);    
}
void translateAstBlockcodeNode(AstBlockcodeNode * node){
    printf("{\n");
    translateAstCodeNode(node->code);
    printf("}");
}
void translateAstCodeNode(AstCodeNode * node){
    if(node != NULL){
        translateAstNodeList(node->statements);
    }
}

void translateAstForNode(AstForNode * node){
    printf("for(");
    translateAstDeclarationNode(node->initialDeclaration);
    translateAstBooleanExpressionNode(node->loopCondition);
    printf(";");
    translateAstForDefinitionNode(node->reDeclaration);
    printf(")");
    translateAstBlockcodeNode(node->blockcode);
}

void translateAstIfNode(AstIfNode * node){
    if(node->type == IF_TYPE) {
        printf("if(");
        translateAstBooleanExpressionNode(node->condition);
        printf(")");
    } else {
        if(node->condition == NULL){
            printf("else ");
        }
        else{
            printf("else if(");
            translateAstBooleanExpressionNode(node->condition);
            printf(")");
        }
    }
    translateAstBlockcodeNode(node->blockcode);
    if(node->next != NULL)
        translateAstIfNode(node->next);  
}

//void translateAstPrintNode(AstPrintNode * node){
//    printf("printf(\"%s);\n",node->string);
//}

//void translateAstGetNode(AstGetNode * node){
//    printf("char 
//}

void translateAstNodeList(AstNodeList * node){
    AstNodeList * it = node; 
    while(it != NULL){
        switch(it->current->type){
            case CODE_TYPE:
            break;
            case ELSE_IF_TYPE:
            case IF_TYPE:
                translateAstIfNode((AstIfNode *) it->current);
            break;
            case FOR_TYPE:
                translateAstForNode((AstForNode * )it->current);
            break;
            case DECLARATION_TYPE:
                translateAstDeclarationNode((AstDeclarationNode*) it->current);
            break;
            //case OUTPUT_TYPE:
            //    translateAstPrintNode((AstPrintNode*)it->current);
            //break;
            //case INPUT_TYPE
            //    translateAstGetNode((AstGetNode*)it->current);
            case DEFINITION_TYPE:
                translateAstDefinitionNode((AstDefinitionNode *) it->current);
            break;
        }
        printf("\n");
        it = it->next;
    }
}

void translateAstDeclarationNode(AstDeclarationNode * node){
    switch (node->dataType)
    {
    case INT_DECLARATION_TYPE:
        printf("int %s",node->name);
        if(node->exp != NULL){
            printf(" = ");
            translateAstArithmeticExpressionNode(node->exp);
        }
        break;
    case STRING_DECLARATION_TYPE:
        printf("char * %s", node->name);
        if(node->str != NULL){
            printf(" = ");
            translateAstConstantNode((AstConstantExpressionNode *)node->str);
        }
        break;
    }
    printf(";");

}

void translateAstForDefinitionNode(AstDefinitionNode * node){
    printf("%s = ", node->name);
    switch (node->dataType)
    {
    case INT_DECLARATION_TYPE:
        translateAstArithmeticExpressionNode((AstArithmeticExpressionNode *) node->exp);
        break;
    case STRING_DECLARATION_TYPE:
        translateAstConstantNode((AstConstantExpressionNode *) node->str);
        break;
    default:
        break;
    }
}

void translateAstDefinitionNode(AstDefinitionNode * node) {
    translateAstForDefinitionNode(node);
    printf(";");
}


void translateAstConstantNode(AstConstantExpressionNode * node){
    printf("%s",node->stringValue);
}

void translateAstNumericExpressionNode(AstNumericExpressionNode * node){
    printf("%d",node->value);
}

void translateAstArithmeticExpressionNode(AstArithmeticExpressionNode * node) {
    if(node->left == NULL && node->right == NULL){
        switch (node->value->type)
        {
        case NUMERIC_TYPE:
            translateAstNumericExpressionNode((AstNumericExpressionNode *)node->value);
            break;
        case CONSTANT_STRING_TYPE:
            translateAstConstantNode((AstConstantExpressionNode *)node->value);
            break;
        default:
            break;
        }
        return;
    }
    translateAstArithmeticExpressionNode(node->left);
    printf(" %s ",node->op);
    translateAstArithmeticExpressionNode(node->right);
}

void translateAstBooleanExpressionNode(AstBooleanExpressionNode * node) { 
    if(node->left == NULL && node->right == NULL){
        switch (node->value->type)
        {
        case NUMERIC_TYPE:
            translateAstNumericExpressionNode((AstNumericExpressionNode *)node->value);
            break;
        case CONSTANT_STRING_TYPE:
            translateAstConstantNode((AstConstantExpressionNode *)node->value);
            break;
        default:
            break;
        }
        return;
    }
    else if(node->left == NULL) {
        printf("%s",node->op);
        translateAstBooleanExpressionNode(node->right);
        return;
    }
    else if(node->right == NULL){
        printf("%s",node->op);
        translateAstBooleanExpressionNode(node->left);
        return;
    }

    translateAstBooleanExpressionNode(node->left);
    printf(" %s ",node->op);
    translateAstBooleanExpressionNode(node->right);
}