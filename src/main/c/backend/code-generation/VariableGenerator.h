#ifndef VARIABLE_GENERATOR_HEADER
#define VARIABLE_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

/**
 * Generates code for a variable declaration using direct output.
 * @param indentationLevel The current indentation level
 * @param variableDeclaration The variable declaration to generate code for
 */
void generateVariableDeclaration(const unsigned int indentationLevel, VariableDeclaration* variableDeclaration);

/**
 * Generates code for a variable declaration list using direct output.
 * @param indentationLevel The current indentation level
 * @param variableDeclarationList The variable declaration list to generate code for
 */
void generateVariableDeclarationList(const unsigned int indentationLevel, VariableDeclarationList* variableDeclarationList);

/**
 * Generates code for an assignment operation using direct output.
 * @param indentationLevel The current indentation level
 * @param assignmentOperation The assignment operation to generate code for
 */
void generateAssignmentOperation(const unsigned int indentationLevel, AssignmentOperation* assignmentOperation);

/**
 * Generates code for privacy modifiers using direct output.
 * @param indentationLevel The current indentation level
 * @param privacyList The privacy modifier list to generate code for
 */
void generatePrivacyModifiers(const unsigned int indentationLevel, PrivacyList* privacyList);

/**
 * Generates code for a variable type using direct output.
 * @param indentationLevel The current indentation level
 * @param variableType The variable type to generate code for
 */
void generateVariableType(const unsigned int indentationLevel, VariableType variableType);

/**
 * Generates code for an object (class/interface type) using direct output.
 * @param indentationLevel The current indentation level
 * @param object The object to generate code for
 */
void generateObject(const unsigned int indentationLevel, Object* object);

#endif
