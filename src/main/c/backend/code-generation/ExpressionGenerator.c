#include "ExpressionGenerator.h"
#include <stdio.h>
#include <stdlib.h>

/* MODULE INTERNAL STATE */
static Logger* _logger = NULL;

/* PRIVATE FUNCTIONS */
static char* _indentation(const unsigned int level);
static char* _expressionTypeToOperator(const ExpressionType type);

void initializeExpressionGeneratorModule() {
    _logger = createLogger("ExpressionGenerator");
}

void shutdownExpressionGeneratorModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

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
            logError(_logger, "Unknown expression type: %d", type);
            return NULL;
    }
}

char* generateConstant(const unsigned int indentationLevel, Constant* constant) {
    char* result = NULL;
    char* indent = _indentation(indentationLevel);
    
    switch (constant->type) {
        case C_INT_TYPE:
            result = concatenate(2, indent, formatString("%d", constant->intValue));
            break;
        case C_CHAR_TYPE:
            result = concatenate(2, indent, formatString("'%c'", constant->charValue));
            break;
        case C_BOOLEAN_TYPE:
            result = concatenate(2, indent, formatString("%s", constant->booleanValue ? "true" : "false"));
            break;
        case C_STRING_TYPE:
            result = concatenate(2, indent, formatString("\"%s\"", constant->stringValue));
            break;
        case C_DOUBLE_TYPE:
            result = concatenate(2, indent, formatString("%lf", constant->doubleValue));
            break;
        case C_FLOAT_TYPE:
            result = concatenate(2, indent, formatString("%f", constant->floatValue));
            break;
        default:
            logError(_logger, "Unknown constant type: %d", constant->type);
            free(indent);
            return NULL;
    }
    
    free(indent);
    return result;
}

char* generateFactor(const unsigned int indentationLevel, Factor* factor) {
    char* result = NULL;
    char* indent = _indentation(indentationLevel);
    
    switch (factor->type) {
        case CONSTANT:
            result = generateConstant(indentationLevel, factor->constant);
            break;
        case EXPRESSION:
            result = generateExpression(indentationLevel, factor->expression);
            break;
        default:
            logError(_logger, "Unknown factor type: %d", factor->type);
            free(indent);
            return NULL;
    }
    
    free(indent);
    return result;
}

char* generateExpression(const unsigned int indentationLevel, Expression* expression) {
    char* result = NULL;
    char* indent = _indentation(indentationLevel);
    
    switch (expression->type) {
        case ADDITION:
        case SUBTRACTION:
        case MULTIPLICATION:
        case DIVISION: {
            char* leftExpr = generateExpression(indentationLevel + 1, expression->leftExpression);
            char* rightExpr = generateExpression(indentationLevel + 1, expression->rightExpression);
            char* operator = _expressionTypeToOperator(expression->type);
            
            if (leftExpr && rightExpr && operator) {
                result = concatenate(8, indent, "(", leftExpr, " ", operator, " ", rightExpr, ")");
            }
            
            free(leftExpr);
            free(rightExpr);
            break;
        }
        case FACTOR:
            result = generateFactor(indentationLevel, expression->factor);
            break;
        default:
            logError(_logger, "Unknown expression type: %d", expression->type);
            free(indent);
            return NULL;
    }
    
    free(indent);
    return result;
} 