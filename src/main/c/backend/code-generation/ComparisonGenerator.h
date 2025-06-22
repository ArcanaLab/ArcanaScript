#ifndef COMPARISON_GENERATOR_HEADER
#define COMPARISON_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

/**
 * Generates code for a comparison expression using direct output.
 * @param indentationLevel The current indentation level
 * @param expression The comparison expression to generate code for
 */
void generateComparisonExpression(const unsigned int indentationLevel, Expression* expression);

/**
 * Generates code for a comparison operator using direct output.
 * @param indentationLevel The current indentation level
 * @param expressionType The type of comparison operator
 */
void generateComparisonOperator(const unsigned int indentationLevel, ExpressionType expressionType);

#endif 