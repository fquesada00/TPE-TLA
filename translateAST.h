#ifndef TRANSLATE_AST_H_
#define TRANSLATE_AST_H_

#include "AST.h"
void translateAstGraphNode(AstGraphNode * node);
void translateAstBlockcodeNode(AstBlockcodeNode * node);
void translateAstCodeNode(AstCodeNode * node);
void translateAstNodeList(AstNodeList * node);
void translateAstDeclarationNode(AstDeclarationNode * node);
void translateAstArithmeticExpressionNode(AstArithmeticExpressionNode * node);
void translateAstConstantNode(AstConstantExpressionNode * node);
#endif