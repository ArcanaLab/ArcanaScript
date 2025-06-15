#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

/** Initialize module's internal state. */
void initializeGeneratorModule();

/** Shutdown module's internal state. */
void shutdownGeneratorModule();

/**
 * Generates the final output using the current compiler state.
 */
void generate(CompilerState * compilerState);

/**
 * Writes the generated output to a file in the test_results directory.
 * @param compilerState The current compiler state
 * @param testName The name of the test (will be used as part of the filename)
 * @return true if the file was written successfully, false otherwise
 */
bool writeGeneratedOutputToFile(CompilerState* compilerState, const char* testName);

#endif
