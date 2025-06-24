#include "ComparisonGenerator.h"
#include "ExpressionGenerator.h"
#include "Generator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

static char* _comparisonTypeToOperator(const ExpressionType type);

static char* _comparisonTypeToOperator(const ExpressionType type) {
    switch (type) {
        case GREATER_TYPE: return ">"; break;
        case GREATER_EQUAL_TYPE: return ">="; break;
        case LESS_TYPE: return "<"; break;
        case LESS_EQUAL_TYPE: return "<="; break;
        case EQUAL_EQUAL_TYPE: return "=="; break;
        case NOT_EQUAL_TYPE: return "!="; break;
        default:
            return NULL;
    }
}

void generateComparisonOperator(const unsigned int indentationLevel, ExpressionType expressionType) {
    char* operator = _comparisonTypeToOperator(expressionType);
    if (operator) {
        generatorOutput(indentationLevel, " %s ", operator);
    }
}

void generateComparisonExpression(const unsigned int indentationLevel, Expression* expression) {
    switch (expression->type) {
        case GREATER_TYPE:
        case GREATER_EQUAL_TYPE:
        case LESS_TYPE:
        case LESS_EQUAL_TYPE:
        case EQUAL_EQUAL_TYPE:
        case NOT_EQUAL_TYPE: {
            char* operator = _comparisonTypeToOperator(expression->type);
            if (operator) {
                generateFactor(indentationLevel, expression->leftFactor);
                generatorOutput(indentationLevel, " %s ", operator);
                generateFactor(indentationLevel, expression->rightFactor);
            }
            break;
        }
        default:
            return;
    }
} 