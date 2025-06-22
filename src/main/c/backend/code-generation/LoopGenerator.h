#ifndef LOOP_GENERATOR_HEADER
#define LOOP_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

/**
 * Generates code for a loop statement (while/for) using direct output.
 * @param indentationLevel The current indentation level
 * @param loop The loop structure to generate code for
 */
void generateLoop(const unsigned int indentationLevel, Loop* loop);

/**
 * Generates code for a while loop using direct output.
 * @param indentationLevel The current indentation level
 * @param loop The while loop to generate code for
 */
void generateWhileLoop(const unsigned int indentationLevel, Loop* loop);

/**
 * Generates code for a for loop using direct output.
 * @param indentationLevel The current indentation level
 * @param loop The for loop to generate code for
 */
void generateForLoop(const unsigned int indentationLevel, Loop* loop);

#endif 