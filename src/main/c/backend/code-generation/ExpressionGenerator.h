#ifndef EXPRESSION_GENERATOR_HEADER
#define EXPRESSION_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

/**
 * Generates code for an expression using direct output.
 * @param indentationLevel The current indentation level
 * @param expression The expression to generate code for
 */
void generateExpression(const unsigned int indentationLevel, Expression* expression);

/**
 * Generates code for a factor using direct output.
 * @param indentationLevel The current indentation level
 * @param factor The factor to generate code for
 */
void generateFactor(const unsigned int indentationLevel, Factor* factor);

/**
 * Generates code for a constant using direct output.
 * @param indentationLevel The current indentation level
 * @param constant The constant to generate code for
 */
void generateConstant(const unsigned int indentationLevel, Constant* constant);

/**
 * Generates code for a function call using direct output.
 * @param indentationLevel The current indentation level
 * @param functionCall The function call to generate code for
 */
void generateFunctionCall(const unsigned int indentationLevel, FunctionCall* functionCall);

// Array helpers
void generateArrayLiteral(const unsigned int indentationLevel, const typeof(((Factor*)0)->arrayLiteral)* arrayLiteral);
void generateArrayLiteralWithType(const unsigned int indentationLevel, const typeof(((Factor*)0)->arrayLiteral)* arrayLiteral, VariableType arrayType);
void generateArrayAccess(const unsigned int indentationLevel, const typeof(((Factor*)0)->arrayAccess)* arrayAccess);

#endif 