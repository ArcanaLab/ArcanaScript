#include "FunctionGenerator.h"
#include "Generator.h"
#include "StructureGenerator.h"
#include "VariableGenerator.h"
#include "../../shared/Logger.h"
#include <stdio.h>
#include <string.h>

void generateLambda(const unsigned int indentationLevel, VariableDeclaration* lambdaDeclaration) {
    if (lambdaDeclaration == NULL || lambdaDeclaration->expression == NULL || lambdaDeclaration->expression->type != LAMBDA) {
        return;
    }

    Lambda* lambda = lambdaDeclaration->expression->lambda;

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

void generateFunction(const unsigned int indentationLevel, VariableDeclaration* functionDeclaration) {
    if (functionDeclaration == NULL || functionDeclaration->expression == NULL || functionDeclaration->expression->type != LAMBDA) {
        return;
    }

    Lambda* lambda = functionDeclaration->expression->lambda;

    // Generate privacy modifiers if present
    if (functionDeclaration->privacyModifierList != NULL) {
        generatePrivacyModifiers(indentationLevel, functionDeclaration->privacyModifierList);
    } else {
        // Default to public if no privacy modifier is specified
        generatorOutput(indentationLevel, "public ");
    }

    // Determine return type from Function<T> object
    char* returnType = "void"; // default
    if (functionDeclaration->object != NULL && functionDeclaration->object->name != NULL) {
        if (strcmp(functionDeclaration->object->name, "Function") == 0 && 
            functionDeclaration->object->genericList != NULL) {
            // Extract the generic type (e.g., Integer from Function<Integer>)
            GenericListNode* genericNode = functionDeclaration->object->genericList->first;
            if (genericNode != NULL) {
                Generic* generic = (Generic*)genericNode->data;
                if (generic != NULL && generic->object != NULL && generic->object->name != NULL) {
                    returnType = generic->object->name;
                }
            }
        }
    }

    // Generate method signature
    generatorOutput(0, "static %s %s(", returnType, functionDeclaration->name);
    
    // Generate method parameters
    if (lambda->variableDeclarationList != NULL) {
        generateVariableDeclarationList(0, lambda->variableDeclarationList);
    }
    
    generatorOutput(0, ")");

    // Generate method body
    if (lambda->block != NULL) {
        generateScope(indentationLevel, lambda->block);
    } else {
        generatorOutput(indentationLevel, " {\n");
        generatorOutput(indentationLevel, "}\n");
    }
    
    generatorOutput(0, "\n");
} 