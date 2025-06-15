#ifndef EXPRESSION_GENERATOR_HEADER
#define EXPRESSION_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"

/** Initialize expression generator module's internal state. */
void initializeExpressionGeneratorModule();

/** Shutdown expression generator module's internal state. */
void shutdownExpressionGeneratorModule();

/**
 * Generates code for an expression.
 * @param indentationLevel The current indentation level
 * @param expression The expression to generate code for
 * @return A string containing the generated code
 */
char* generateExpression(const unsigned int indentationLevel, Expression* expression);

/**
 * Generates code for a factor.
 * @param indentationLevel The current indentation level
 * @param factor The factor to generate code for
 * @return A string containing the generated code
 */
char* generateFactor(const unsigned int indentationLevel, Factor* factor);

/**
 * Generates code for a constant.
 * @param indentationLevel The current indentation level
 * @param constant The constant to generate code for
 * @return A string containing the generated code
 */
char* generateConstant(const unsigned int indentationLevel, Constant* constant);

/**
 * Writes the generated expression to a file in the test_results directory.
 * @param expression The expression to generate code for
 * @param testName The name of the test (will be used as part of the filename)
 * @return true if the file was written successfully, false otherwise
 */
bool writeExpressionToFile(Expression* expression, const char* testName);

#endif 