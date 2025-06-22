#include "AssignmentGenerator.h"
#include "Generator.h"
#include "ExpressionGenerator.h"
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
    
    // Handle the case where we have a variable name (not a variable declaration)
    if (assignmentOperation->name != NULL) {
        // Generate variable name
        generatorOutput(indentationLevel, "%s", assignmentOperation->name);
        
        // Generate assignment operator
        _generateAssignmentOperator(indentationLevel, assignmentOperation->assignmentOperator);
        
        // Generate the expression (which should be a constant in our simple case) 
        if (assignmentOperation->expression != NULL) {
            generateExpression(indentationLevel, assignmentOperation->expression);
        }
        
        // End with semicolon and newline
        generatorOutput(indentationLevel, ";\n");
    }

} 