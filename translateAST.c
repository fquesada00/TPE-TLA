#include "translateAST.h"
#include "AST.h"
#include <stdio.h>
#include <stdlib.h>

void translateAstGraphNode(AstGraphNode * node){
    printf("int main()");
    translateAstBlockcodeNode(node->blockcode);
}
void translateAstBlockcodeNode(AstBlockcodeNode * node){
    printf("{\n");
    translateAstCodeNode(node->code);
    printf("\n}");
}
void translateAstCodeNode(AstCodeNode * node){
    translateAstNodeList(node->statements);
}
void translateAstNodeList(AstNodeList * node){
    AstNodeList * it = node; 
    while(it != NULL){
        switch(it->current->type){
            case CODE_TYPE:
            break;
            case IF_TYPE:
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
        printf("int %s",node->name);
        if(node->exp != NULL){
            printf(" = ");
            translateAstArithmeticExpressionNode(node->exp);
        }
        break;
    case STRING_DECLARATION_TYPE:
        printf("char * %s", node->name);
        break;
    }
    printf(";\n");

}
void translateAstArithmeticExpressionNode(AstArithmeticExpressionNode * node) {
    if(node->left == NULL && node->right == NULL){
        printf("%d",node->value);
        return;
    }
    translateAstArithmeticExpressionNode(node->left);
    printf("%s",node->op);
    translateAstArithmeticExpressionNode(node->right);
}