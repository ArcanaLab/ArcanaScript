#include "ExpressionGenerator.h"
#include "Generator.h"
#include "VariableGenerator.h"
#include "FunctionGenerator.h"
#include "ClassGenerator.h"
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
        case MODULE: return "%";
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
        case ARRAY_LITERAL:
            generateArrayLiteral(indentationLevel, &factor->arrayLiteral);
            break;
        case ARRAY_ACCESS:
            generateArrayAccess(indentationLevel, &factor->arrayAccess);
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
        case DIVISION:
        case MODULE: {
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
    
    // Special handling for 'print' function
    if (functionCall->name != NULL && strcmp(functionCall->name, "print") == 0) {
        generatorOutput(indentationLevel, "System.out.println(");
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
    } else {
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
}

// Helper: Generate array literal as new Type[] { ... }
void generateArrayLiteral(const unsigned int indentationLevel, const typeof(((Factor*)0)->arrayLiteral)* arrayLiteral) {
    // TODO: Type inference for array type (for now, use int[] as default)
    // In a real implementation, you would pass the type down or infer from context
    generatorOutput(indentationLevel, "new int[] {");
    if (arrayLiteral && arrayLiteral->elements) {
        ExpressionNode* current = arrayLiteral->elements->first;
        bool first = true;
        while (current) {
            if (!first) generatorOutput(0, ", ");
            generateExpression(0, (Expression*)current->data);
            first = false;
            current = current->next;
        }
    }
    generatorOutput(0, "}");
}

// Helper: Generate array literal with specific type
void generateArrayLiteralWithType(const unsigned int indentationLevel, const typeof(((Factor*)0)->arrayLiteral)* arrayLiteral, VariableType arrayType) {
    char* typeString = VariableTypeToString(arrayType);
    generatorOutput(indentationLevel, "new %s {", typeString);
    if (arrayLiteral && arrayLiteral->elements) {
        ExpressionNode* current = arrayLiteral->elements->first;
        bool first = true;
        while (current) {
            if (!first) generatorOutput(0, ", ");
            generateExpression(0, (Expression*)current->data);
            first = false;
            current = current->next;
        }
    }
    generatorOutput(0, "}");
}

// Helper: Generate array access as name[index]
void generateArrayAccess(const unsigned int indentationLevel, const typeof(((Factor*)0)->arrayAccess)* arrayAccess) {
    if (arrayAccess && arrayAccess->arrayName && arrayAccess->index) {
        generatorOutput(indentationLevel, "%s[", arrayAccess->arrayName);
        generateExpression(0, arrayAccess->index);
        generatorOutput(0, "]");
    }
} 