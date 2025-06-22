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

    // Generate return type - for now, use void since ArcanaScript lambdas don't have explicit return types
    generatorOutput(indentationLevel, "void");

    // Generate function name
    generatorOutput(0, " %s(", functionDeclaration->name);

    // Generate function parameters
    if (lambda->variableDeclarationList != NULL) {
        generateVariableDeclarationList(0, lambda->variableDeclarationList);
    }

    generatorOutput(0, ")");

    // Generate function body
    if (lambda->block != NULL) {
        generateScope(indentationLevel, lambda->block);
    } else {
        generatorOutput(indentationLevel, " {\n");
        generatorOutput(indentationLevel, "}\n");
    }
    
    generatorOutput(0, "\n");
} 