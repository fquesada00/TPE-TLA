#ifndef TRANSLATE_AST_H_
#define TRANSLATE_AST_H_

#include "AST.h"
void translateAstGraphNode(AstGraphNode * node);
void translateAstBlockcodeNode(AstBlockcodeNode * node);
void translateAstCodeNode(AstCodeNode * node);
void translateAstNodeList(AstNodeList * node);
void translateAstDeclarationNode(AstDeclarationNode * node);
void translateAstBooleanExpressionNode(AstBooleanExpressionNode * node);
void translateAstArithmeticExpressionNode(AstArithmeticExpressionNode * node);
void translateAstConstantNode(AstConstantExpressionNode * node);
void translateAstDefinitionNode(AstDefinitionNode * node);
void translateAstForNode(AstForNode * node);
void translateAstNumericExpressionNode(AstNumericExpressionNode * node);
void translateAstForDefinitionNode(AstDefinitionNode * node);
void translateAstPrintNode(AstPrintNode * node);
void translateInput(char * name);
#endif