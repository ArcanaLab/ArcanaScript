#ifndef ASSIGNMENT_GENERATOR_HEADER
#define ASSIGNMENT_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

/**
 * Generates code for a simple assignment operation (variable = constant) using direct output.
 * @param indentationLevel The current indentation level
 * @param assignmentOperation The assignment operation to generate code for
 */
void generateSimpleAssignment(const unsigned int indentationLevel, AssignmentOperation* assignmentOperation);


#endif
