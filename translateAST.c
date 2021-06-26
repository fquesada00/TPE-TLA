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
    printf("\n}");
}
void translateAstCodeNode(AstCodeNode * node){
    if(node != NULL){
        translateAstNodeList(node->statements);
    }
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
            case DECLARATION_TYPE:
                translateAstDeclarationNode((AstDeclarationNode*) it->current);
            break;
        }
        it = it->next;
    }
}

void translateAstDeclarationNode(AstDeclarationNode * node){
    switch (node->data_type)
    {
    case INT_DECLARATION_TYPE:
        printf("\tint %s",node->name);
        if(node->exp != NULL){
            printf(" = ");
            translateAstArithmeticExpressionNode(node->exp);
        }
        break;
    case STRING_DECLARATION_TYPE:
        printf("\tchar * %s", node->name);
        if(node->str != NULL){
            printf(" = ");
            translateAstConstantNode((AstConstantExpressionNode *)node->str);
        }
        break;
    }
    printf(";\n");

}

void translateAstConstantNode(AstConstantExpressionNode * node){
    printf("%s",node->stringValue);
}

void translateAstArithmeticExpressionNode(AstArithmeticExpressionNode * node) {
    if(node->left == NULL && node->right == NULL){
        printf("%d",node->value);
        return;
    }
    translateAstArithmeticExpressionNode(node->left);
    printf(" %s ",node->op);
    translateAstArithmeticExpressionNode(node->right);
}

void translateAstBooleanExpressionNode(AstBooleanExpressionNode * node) { 
    if(node->left == NULL && node->right == NULL){
        printf("%d",node->value);
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