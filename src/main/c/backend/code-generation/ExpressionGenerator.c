#include "ExpressionGenerator.h"
#include "Generator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

/* PRIVATE FUNCTIONS */
static char* _indentation(const unsigned int level);
static char* _expressionTypeToOperator(const ExpressionType type);

static char* _indentation(const unsigned int level) {
    const char indentationCharacter = ' ';
    const char indentationSize = 4;
    return indentation(indentationCharacter, level, indentationSize);
}

static char* _expressionTypeToOperator(const ExpressionType type) {
    switch (type) {
        case ADDITION: return "+";
        case SUBTRACTION: return "-";
        case MULTIPLICATION: return "*";
        case DIVISION: return "/";
        default:
            return NULL;
    }
}

void generateConstant(const unsigned int indentationLevel, Constant* constant) {
    switch (constant->type) {
        case C_INT_TYPE:
            generatorOutput(indentationLevel, "%d", constant->intValue);
            break;
        case C_CHAR_TYPE:
            generatorOutput(indentationLevel, "'%c'", constant->charValue);
            break;
        case C_BOOLEAN_TYPE:
            generatorOutput(indentationLevel, "%s", constant->booleanValue ? "true" : "false");
            break;
        case C_STRING_TYPE:
            generatorOutput(indentationLevel, "\"%s\"", constant->stringValue);
            break;
        case C_DOUBLE_TYPE:
            generatorOutput(indentationLevel, "%lf", constant->doubleValue);
            break;
        case C_FLOAT_TYPE:
            generatorOutput(indentationLevel, "%f", constant->floatValue);
            break;
        default:
            break;
    }
}

void generateFactor(const unsigned int indentationLevel, Factor* factor) {
    switch (factor->type) {
        case CONSTANT:
            generateConstant(indentationLevel, factor->constant);
            break;
        case EXPRESSION:
            generateExpression(indentationLevel, factor->expression);
            break;
        case VARIABLE_TYPE:
            generatorOutput(indentationLevel, "%s", factor->variable);
            break;
        case INCREMENT_TYPE:
            generatorOutput(indentationLevel, "%s++", factor->variable);
            break;
        case DECREMENT_TYPE:
            generatorOutput(indentationLevel, "%s--", factor->variable);
            break;
        default:
            break;
    }
}

void generateExpression(const unsigned int indentationLevel, Expression* expression) {
    switch (expression->type) {
        case ADDITION:
        case SUBTRACTION:
        case MULTIPLICATION:
        case DIVISION: {
            char* operator = _expressionTypeToOperator(expression->type);
            if (operator) {
                generatorOutput(indentationLevel, "(");
                generateExpression(indentationLevel + 1, expression->leftExpression);
                generatorOutput(indentationLevel + 1, " %s ", operator);
                generateExpression(indentationLevel + 1, expression->rightExpression);
                generatorOutput(indentationLevel, ")");
            }
            break;
        }
        case FACTOR:
            generateFactor(indentationLevel, expression->factor);
            break;
        case FUNCTION_CALL:
            generateFunctionCall(indentationLevel, expression->functionCall);
            break;
        default:
            break;
    }
}

void generateFunctionCall(const unsigned int indentationLevel, FunctionCall* functionCall) {
    if (functionCall == NULL) {
        return;
    }
    
    // Generate function name
    generatorOutput(indentationLevel, "%s(", functionCall->name);
    
    // Generate function arguments
    if (functionCall->expressionList != NULL) {
        ExpressionNode* currentExpr = functionCall->expressionList->first;
        bool first = true;
        
        while (currentExpr != NULL) {
            Expression* expression = (Expression*)currentExpr->data;
            
            if (!first) {
                generatorOutput(0, ", ");
            }
            
            generateExpression(0, expression);
            
            first = false;
            currentExpr = currentExpr->next;
        }
    }
    
    generatorOutput(0, ")");
} 