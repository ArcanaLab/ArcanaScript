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
char* VariableTypeToString(const VariableType type);
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

char* VariableTypeToString(const VariableType type) {
    switch (type) {
        case V_INT: return "int";
        case V_CHAR: return "char";
        case V_BOOLEAN: return "boolean";
        case V_STRING: return "String";
        case V_DOUBLE: return "double";
        case V_FLOAT: return "float";
        case V_LONG: return "long";
        case V_SHORT: return "short";
        case V_INT_ARRAY: return "int[]";
        case V_CHAR_ARRAY: return "char[]";
        case V_BOOLEAN_ARRAY: return "boolean[]";
        case V_STRING_ARRAY: return "String[]";
        case V_DOUBLE_ARRAY: return "double[]";
        case V_FLOAT_ARRAY: return "float[]";
        case V_LONG_ARRAY: return "long[]";
        case V_SHORT_ARRAY: return "short[]";
        case V_OBJECT_ARRAY: return "Object[]";
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
    char* typeString = VariableTypeToString(variableType);
    generatorOutput(indentationLevel, "%s", typeString);
}

static void generateObject(const unsigned int indentationLevel, Object* object) {
    if (object == NULL) {
        return;
    }
    if (object->name != NULL && strcmp(object->name, "bool") == 0) {
        generatorOutput(indentationLevel, "boolean");
    } else {
        generatorOutput(indentationLevel, "%s", object->name);
    }
}

void generateVariableDeclaration(const unsigned int indentationLevel, VariableDeclaration* variableDeclaration) {
    if (variableDeclaration == NULL) {
        return;
    }
    
    if (variableDeclaration->expression != NULL && variableDeclaration->expression->type == LAMBDA) {
        if (variableDeclaration->type == OBJECT && variableDeclaration->object != NULL) {
            generateFunction(indentationLevel, variableDeclaration);
        } else {
            generateLambda(indentationLevel, variableDeclaration);
        }
        return;
    }

    if (variableDeclaration->privacyModifierList != NULL) {
        generatePrivacyModifiers(indentationLevel, variableDeclaration->privacyModifierList);
    }
    
    if (variableDeclaration->type == OBJECT && variableDeclaration->object != NULL) {
        generateObject(indentationLevel, variableDeclaration->object);
    } else if (variableDeclaration->type == V_OBJECT_ARRAY && variableDeclaration->object != NULL) {
        generatorOutput(indentationLevel, "%s[]", variableDeclaration->object->name);
    } else {
        generateVariableType(indentationLevel, variableDeclaration->type);
    }
    
    generatorOutput(indentationLevel, " %s", variableDeclaration->name);
    
    if (variableDeclaration->expression != NULL) {
        generatorOutput(indentationLevel, " = ");
        
        if (variableDeclaration->expression->type == FACTOR && 
            variableDeclaration->expression->factor->type == ARRAY_LITERAL &&
            (variableDeclaration->type == V_INT_ARRAY || 
             variableDeclaration->type == V_STRING_ARRAY || 
             variableDeclaration->type == V_FLOAT_ARRAY || 
             variableDeclaration->type == V_BOOLEAN_ARRAY || 
             variableDeclaration->type == V_CHAR_ARRAY || 
             variableDeclaration->type == V_DOUBLE_ARRAY || 
             variableDeclaration->type == V_LONG_ARRAY || 
             variableDeclaration->type == V_SHORT_ARRAY ||
             variableDeclaration->type == V_OBJECT_ARRAY)) {
            generateArrayLiteralWithType(indentationLevel, &variableDeclaration->expression->factor->arrayLiteral, variableDeclaration->type);
        } else {
            generateExpression(indentationLevel, variableDeclaration->expression);
        }
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
        } else if (variableDeclaration->type == V_OBJECT_ARRAY && variableDeclaration->object != NULL) {
            generatorOutput(indentationLevel, "%s[]", variableDeclaration->object->name);
        } else {
            generateVariableType(indentationLevel, variableDeclaration->type);
        }
        
        generatorOutput(indentationLevel, " %s", variableDeclaration->name);
        
        first = false;
        currentVar = currentVar->next;
    }
}
