#ifndef CONDITIONAL_GENERATOR_HEADER
#define CONDITIONAL_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

/**
 * Generates code for a conditional statement (if/else) using direct output.
 * @param indentationLevel The current indentation level
 * @param conditional The conditional structure to generate code for
 */
void generateConditional(const unsigned int indentationLevel, Conditional* conditional);

/**
 * Generates code for an if statement using direct output.
 * @param indentationLevel The current indentation level
 * @param conditional The if conditional to generate code for
 */
void generateIfStatement(const unsigned int indentationLevel, Conditional* conditional);

/**
 * Generates code for an else statement using direct output.
 * @param indentationLevel The current indentation level
 * @param conditional The else conditional to generate code for
 */
void generateElseStatement(const unsigned int indentationLevel, Conditional* conditional);

#endif 