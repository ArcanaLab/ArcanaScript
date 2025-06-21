#include "VariableGenerator.h"
#include "Generator.h"
#include "ExpressionGenerator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

/* PRIVATE FUNCTIONS */
static char* _assignmentOperatorToString(const AssignmentOperatorType type);
static char* _variableTypeToString(const VariableType type);
static char* _privacyTypeToString(const PrivacyType type);

static char* _assignmentOperatorToString(const AssignmentOperatorType type) {
    switch (type) {
        case ASSIGN_TYPE: return "=";
        case ADD_ASSIGN_TYPE: return "+=";
        case SUB_ASSIGN_TYPE: return "-=";
        case MUL_ASSIGN_TYPE: return "*=";
        default:
            return "=";
    }
}

static char* _variableTypeToString(const VariableType type) {
    switch (type) {
        case V_INT: return "int";
        case V_CHAR: return "char";
        case V_BOOLEAN: return "boolean";
        case V_STRING: return "string";
        case V_DOUBLE: return "double";
        case V_FLOAT: return "float";
        case V_LONG: return "long";
        case V_SHORT: return "short";
        case OBJECT: return "object";
        default:
            return "unknown";
    }
}

static char* _privacyTypeToString(const PrivacyType type) {
    switch (type) {
        case PRIVATE_A: return "private";
        case PUBLIC_A: return "public";
        case PROTECTED_A: return "protected";
        case CONST_A: return "const";
        case STATIC_A: return "static";
        default:
            return "";
    }
}

void generatePrivacyModifiers(const unsigned int indentationLevel, PrivacyList* privacyList) {
    if (privacyList == NULL) {
        return;
    }
    
    PrivacyNode* currentPrivacy = privacyList->first;
    while (currentPrivacy != NULL) {
        PrivacyModifier* privacy = (PrivacyModifier*)currentPrivacy->data;
        if (privacy != NULL) {
            char* privacyString = _privacyTypeToString(privacy->type);
            generatorOutput(indentationLevel, "%s ", privacyString);
        }
        currentPrivacy = currentPrivacy->next;
    }
}

void generateVariableType(const unsigned int indentationLevel, VariableType variableType) {
    char* typeString = _variableTypeToString(variableType);
    generatorOutput(indentationLevel, "%s", typeString);
}

void generateObject(const unsigned int indentationLevel, Object* object) {
    if (object == NULL) {
        return;
    }
    
    generatorOutput(indentationLevel, "%s", object->name);
    
    // TODO: Hacer bien esta funcion cuando lleguemos al tema Objets.
}

void generateVariableDeclaration(const unsigned int indentationLevel, VariableDeclaration* variableDeclaration) {
    if (variableDeclaration == NULL) {
        return;
    }
    
    // Generate privacy modifiers if any
    if (variableDeclaration->privacyModifierList != NULL) {
        generatePrivacyModifiers(indentationLevel, variableDeclaration->privacyModifierList);
    }
    
    // Generate variable name
    generatorOutput(indentationLevel, "%s: ", variableDeclaration->name);
    
    // Generate variable type
    if (variableDeclaration->type == OBJECT && variableDeclaration->object != NULL) {
        generateObject(indentationLevel, variableDeclaration->object);
    } else {
        generateVariableType(indentationLevel, variableDeclaration->type);
    }
    
    // Generate assignment if present
    if (variableDeclaration->expression != NULL) {
        generatorOutput(indentationLevel, " = ");
        generateExpression(indentationLevel, variableDeclaration->expression);
    }
    
    generatorOutput(indentationLevel, ";\n");
}

void generateVariableDeclarationList(const unsigned int indentationLevel, VariableDeclarationList* variableDeclarationList) {
    if (variableDeclarationList == NULL) {
        return;
    }
    
    VariableDeclarationNode* currentVar = variableDeclarationList->first;
    bool first = true;
    
    while (currentVar != NULL) {
        VariableDeclaration* variableDeclaration = (VariableDeclaration*)currentVar->data;
        
        if (!first) {
            generatorOutput(indentationLevel, ", ");
        }
        
        // Generate variable name
        generatorOutput(indentationLevel, "%s: ", variableDeclaration->name);
        
        // Generate variable type
        if (variableDeclaration->type == OBJECT && variableDeclaration->object != NULL) {
            generateObject(indentationLevel, variableDeclaration->object);
        } else {
            generateVariableType(indentationLevel, variableDeclaration->type);
        }
        
        first = false;
        currentVar = currentVar->next;
    }
}

void generateAssignmentOperation(const unsigned int indentationLevel, AssignmentOperation* assignmentOperation) {
    if (assignmentOperation == NULL) {
        return;
    }
    
    if (assignmentOperation->variableDeclaration != NULL) {
        generateVariableDeclaration(indentationLevel, assignmentOperation->variableDeclaration);
    } else {
        generatorOutput(indentationLevel, "%s ", assignmentOperation->name);
        
        char* operator = _assignmentOperatorToString(assignmentOperation->assignmentOperator);
        generatorOutput(indentationLevel, "%s ", operator);
        
        if (assignmentOperation->expression != NULL) {
            if (assignmentOperation->expression->type == FACTOR) {
                Factor* factor = assignmentOperation->expression->factor;
                if (factor->type == CONSTANT) {
                    generateConstant(0, factor->constant);
                } else {
                    generateExpression(0, assignmentOperation->expression);
                }
            } else {
                generateExpression(0, assignmentOperation->expression);
            }
        }
        
        generatorOutput(indentationLevel, ";\n");
    }
}
