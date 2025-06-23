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

/* PRIVATE FUNCTIONS */
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
    
    // Handle array assignment: if name and index are present (arrayAccess)
    if (assignmentOperation->name != NULL && assignmentOperation->expression != NULL && assignmentOperation->assignmentOperator == ASSIGN_TYPE && assignmentOperation->index != NULL) {
        // Generate array assignment: name[index] = value;
        generatorOutput(indentationLevel, "%s[", assignmentOperation->name);
        generateExpression(0, assignmentOperation->index);
        generatorOutput(0, "] = ");
        generateExpression(0, assignmentOperation->expression);
        generatorOutput(0, ";\n");
        return;
    }
    // Handle the case where we have a variable name (not a variable declaration)
    if (assignmentOperation->name != NULL) {
        // Generate variable name
        generatorOutput(indentationLevel, "%s", assignmentOperation->name);
        
        // Generate assignment operator
        _generateAssignmentOperator(indentationLevel, assignmentOperation->assignmentOperator);
        
        // Check if the right-hand side is a lambda expression
        if (assignmentOperation->expression != NULL && assignmentOperation->expression->type == LAMBDA) {
            // Generate lambda expression
            Lambda* lambda = assignmentOperation->expression->lambda;
            
            // Generate lambda parameters
            generatorOutput(0, "(");
            if (lambda->variableDeclarationList != NULL) {
                generateVariableDeclarationList(0, lambda->variableDeclarationList);
            }
            generatorOutput(0, ") -> ");
            
            // Generate lambda body
            if (lambda->block != NULL) {
                generateScope(0, lambda->block);
            } else {
                generatorOutput(0, "{}");
            }
        } else {
            // Generate the expression (which should be a constant in our simple case) 
            if (assignmentOperation->expression != NULL) {
                generateExpression(indentationLevel, assignmentOperation->expression);
            }
        }
        
        // End with semicolon and newline
        generatorOutput(indentationLevel, ";\n");
    }
} 