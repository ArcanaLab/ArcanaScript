#include "ConditionalGenerator.h"
#include "ComparisonGenerator.h"
#include "StructureGenerator.h"
#include "Generator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

void generateIfStatement(const unsigned int indentationLevel, Conditional* conditional) {
    generatorOutput(indentationLevel, "if (");
    
    if (conditional->expression != NULL) {
        generateComparisonExpression(indentationLevel + 1, conditional->expression);
    }
    
    generatorOutput(indentationLevel, ") ");
    
    if (conditional->block != NULL) {
        generateScope(indentationLevel, conditional->block);
    }
}

void generateElseStatement(const unsigned int indentationLevel, Conditional* conditional) {
    generatorOutput(indentationLevel, "else ");
    
    if (conditional->block != NULL) {
        generateScope(indentationLevel, conditional->block);
    }
}

void generateConditional(const unsigned int indentationLevel, Conditional* conditional) {
    if (conditional == NULL) {
        return;
    }
    
    switch (conditional->ConditionalType) {
        case IF_TYPE:
            generateIfStatement(indentationLevel, conditional);
            break;
        case ELSE_TYPE:
            generateElseStatement(indentationLevel, conditional);
            break;
        case ELSE_IF_TYPE:
            generatorOutput(indentationLevel, "else ");
            generateIfStatement(indentationLevel, conditional);
            break;
        default:
            break;
    }
    
    if (conditional->nextConditional != NULL) {
        generateConditional(indentationLevel, conditional->nextConditional);
    }
} 