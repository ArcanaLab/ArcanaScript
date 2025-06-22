#ifndef FUNCTION_GENERATOR_HEADER
#define FUNCTION_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

void generateLambda(const unsigned int indentationLevel, VariableDeclaration* lambdaDeclaration);
void generateFunction(const unsigned int indentationLevel, VariableDeclaration* functionDeclaration);

#endif 