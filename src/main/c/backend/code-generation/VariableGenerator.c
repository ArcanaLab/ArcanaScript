#include "VariableGenerator.h"
#include "Generator.h"
#include "ExpressionGenerator.h"
#include "FunctionGenerator.h"
#include "ClassGenerator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

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
        case V_STRING: return "String";
        case V_DOUBLE: return "double";
        case V_FLOAT: return "float";
        case V_LONG: return "long";
        case V_SHORT: return "short";
        case OBJECT: return "Object";
        default:
            return "Object";
    }
}

static char* _privacyTypeToString(const PrivacyType type) {
    switch (type) {
        case PRIVATE_A: return "private";
        case PUBLIC_A: return "public";
        case PROTECTED_A: return "protected";
        case CONST_A: return "final";
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

void generateVariableDeclaration(const unsigned int indentationLevel, VariableDeclaration* variableDeclaration) {
    if (variableDeclaration == NULL) {
        return;
    }
    
    // Check if it is a function declaration (variable assigned a lambda)
    if (variableDeclaration->expression != NULL && variableDeclaration->expression->type == LAMBDA) {
        // Check if this is a function with explicit return type (has object type)
        if (variableDeclaration->type == OBJECT && variableDeclaration->object != NULL) {
            // This is a function declaration with return type
            generateFunction(indentationLevel, variableDeclaration);
        } else {
            // This is a lambda assignment
            generateLambda(indentationLevel, variableDeclaration);
        }
        return;
    }

    if (variableDeclaration->privacyModifierList != NULL) {
        generatePrivacyModifiers(indentationLevel, variableDeclaration->privacyModifierList);
    }
    
    if (variableDeclaration->type == OBJECT && variableDeclaration->object != NULL) {
        generateObject(indentationLevel, variableDeclaration->object);
    } else {
        generateVariableType(indentationLevel, variableDeclaration->type);
    }
    
    generatorOutput(indentationLevel, " %s", variableDeclaration->name);
    
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
        
        if (variableDeclaration->type == OBJECT && variableDeclaration->object != NULL) {
            generateObject(indentationLevel, variableDeclaration->object);
        } else {
            generateVariableType(indentationLevel, variableDeclaration->type);
        }
        
        generatorOutput(indentationLevel, " %s", variableDeclaration->name);
        
        first = false;
        currentVar = currentVar->next;
    }
}
