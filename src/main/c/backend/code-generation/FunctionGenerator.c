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
    generatorOutput(indentationLevel, "(args) ->{");
    if (lambda->variableDeclarationList != NULL) {
        generateVariableDeclarationList(0, lambda->variableDeclarationList);
    }
    if (lambda->block != NULL) {
        generateScope(indentationLevel, lambda->block);
    } 
    generatorOutput(indentationLevel, "}");

    
}
void generateArgumentList(const unsigned int indentationLevel, VariableDeclarationList* argumentList) {
    if (argumentList == NULL || argumentList->size == 0) {
        return;
    }

    Node* currentNode = argumentList->first;
    int currentIndex = 0;
    while (currentNode != NULL) {
        
        VariableDeclaration* argument = (VariableDeclaration*)currentNode->data;
        if (argument != NULL) {
            char * typeName = VariableTypeToString(argument->type);
            if (argument->type == OBJECT && argument->object != NULL) {
                typeName = argument->object->name;
            }
            generateVariableDeclaration(indentationLevel, argument);
            generatorOutput(0, "(%s)args[%d]", typeName, currentIndex);

            currentNode = currentNode->next;
            if (currentNode != NULL) {
                generatorOutput(0, "; ");
            }
        } else {
            currentNode = currentNode->next;
        }
        currentIndex++;
    }
}
void generateFunction(const unsigned int indentationLevel, VariableDeclaration* functionDeclaration) {
    if (functionDeclaration == NULL || functionDeclaration->expression == NULL || functionDeclaration->expression->type != LAMBDA) {
        return;
    }

    Lambda* lambda = functionDeclaration->expression->lambda;
    bool hasOverride = false;
    bool hasStatic = false;
    
    if (functionDeclaration->privacyModifierList != NULL) {
        PrivacyNode* currentPrivacy = functionDeclaration->privacyModifierList->first;
        while (currentPrivacy != NULL) {
            PrivacyModifier* privacy = (PrivacyModifier*)currentPrivacy->data;
            if (privacy != NULL) {
                if (privacy->type == OVERRIDE_A) {
                    hasOverride = true;
                } else if (privacy->type == STATIC_A) {
                    hasStatic = true;
                }
            }
            currentPrivacy = currentPrivacy->next;
        }
    }

    if (functionDeclaration->privacyModifierList != NULL) {
        generatePrivacyModifiers(indentationLevel, functionDeclaration->privacyModifierList);
    } else {
        generatorOutput(indentationLevel, "public ");
    }
    if (hasOverride) {
        generatorOutput(indentationLevel, "@Override\n");
    }
    char* returnType = "void"; 
    if (functionDeclaration->object != NULL && functionDeclaration->object->name != NULL) {
        if (strcmp(functionDeclaration->object->name, "Function") == 0 && 
            functionDeclaration->object->genericList != NULL) {
            GenericListNode* genericNode = functionDeclaration->object->genericList->first;
            if (genericNode != NULL) {
                Generic* generic = (Generic*)genericNode->data;
                if (generic != NULL && generic->object != NULL && generic->object->name != NULL) {
                    returnType = generic->object->name;
                }
            }
        }
    }

    if (hasStatic && !hasOverride) {
        generatorOutput(0, "static %s %s(", returnType, functionDeclaration->name);
    } else {
        generatorOutput(0, "%s %s(", returnType, functionDeclaration->name);
    }

    if (lambda->variableDeclarationList != NULL) {
        generateVariableDeclarationList(0, lambda->variableDeclarationList);
    }

    generatorOutput(0, ")");
    
    if (lambda->block != NULL) {
        generateScope(indentationLevel, lambda->block);
    } else {
        generatorOutput(indentationLevel, " {\n");
        generatorOutput(indentationLevel, "}\n");
    }
    
    generatorOutput(0, "\n");
} 