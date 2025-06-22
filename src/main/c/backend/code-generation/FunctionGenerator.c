#include "FunctionGenerator.h"
#include "Generator.h"
#include "StructureGenerator.h"
#include "VariableGenerator.h"
#include "../../shared/Logger.h"
#include <stdio.h>

void generateFunction(const unsigned int indentationLevel, VariableDeclaration* functionDeclaration) {
    if (functionDeclaration == NULL || functionDeclaration->expression == NULL || functionDeclaration->expression->type != LAMBDA) {
        return;
    }

    Lambda* lambda = functionDeclaration->expression->lambda;

    // Generate lambda parameters
    generatorOutput(indentationLevel, "(");
    if (lambda->variableDeclarationList != NULL) {
        generateVariableDeclarationList(0, lambda->variableDeclarationList);
    }
    generatorOutput(0, ") -> ");

    // Generate lambda body
    if (lambda->block != NULL) {
        generateScope(indentationLevel, lambda->block);
    } else {
        generatorOutput(indentationLevel, "{}");
    }
} 