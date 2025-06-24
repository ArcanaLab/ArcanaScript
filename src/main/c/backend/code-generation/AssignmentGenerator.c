#include "AssignmentGenerator.h"
#include "Generator.h"
#include "ExpressionGenerator.h"
#include "FunctionGenerator.h"
#include "VariableGenerator.h"
#include "StructureGenerator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

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


void _generateAssignmentOperator(const unsigned int indentationLevel, AssignmentOperatorType operatorType) {
    char* operator = _assignmentOperatorToString(operatorType);
    generatorOutput(indentationLevel, " %s ", operator);
}

void generateSimpleAssignment(const unsigned int indentationLevel, AssignmentOperation* assignmentOperation) {
    if (assignmentOperation == NULL) {
        return;
    }
    
    if (assignmentOperation->name != NULL && assignmentOperation->expression != NULL && assignmentOperation->assignmentOperator == ASSIGN_TYPE && assignmentOperation->index != NULL) {
        generatorOutput(indentationLevel, "%s[", assignmentOperation->name);
        generateExpression(0, assignmentOperation->index);
        generatorOutput(0, "] = ");
        generateExpression(0, assignmentOperation->expression);
        generatorOutput(0, ";\n");
        return;
    }
    if (assignmentOperation->name != NULL) {
        generatorOutput(indentationLevel, "%s", assignmentOperation->name);
        
        _generateAssignmentOperator(indentationLevel, assignmentOperation->assignmentOperator);
        
        if (assignmentOperation->expression != NULL && assignmentOperation->expression->type == LAMBDA) {
            Lambda* lambda = assignmentOperation->expression->lambda;
            generatorOutput(0, "(");
            if (lambda->variableDeclarationList != NULL) {
                generateVariableDeclarationList(0, lambda->variableDeclarationList);
            }
            generatorOutput(0, ") -> ");
            if (lambda->block != NULL) {
                generateScope(0, lambda->block);
            } else {
                generatorOutput(0, "{}");
            }
        } else {
            if (assignmentOperation->expression != NULL) {
                generateExpression(indentationLevel, assignmentOperation->expression);
            }
        }
        generatorOutput(indentationLevel, ";\n");
    }
} 