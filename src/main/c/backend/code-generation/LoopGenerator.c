#include "LoopGenerator.h"
#include "ExpressionGenerator.h"
#include "StructureGenerator.h"
#include "Generator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

void generateWhileLoop(const unsigned int indentationLevel, Loop* loop) {
    generatorOutput(indentationLevel, "while (");
    
    if (loop->expression != NULL) {
        generateExpression(indentationLevel + 1, loop->expression);
    }
    
    generatorOutput(indentationLevel, ") ");
    
    if (loop->block != NULL) {
        generateScope(indentationLevel, loop->block);
    }
}

void generateForLoop(const unsigned int indentationLevel, Loop* loop) {
    generatorOutput(indentationLevel, "for (");
    
    if (loop->itemName != NULL) {
        generatorOutput(indentationLevel, "%s", loop->itemName);
    }
    
    generatorOutput(indentationLevel, " : ");
    
    if (loop->collectionName != NULL) {
        generatorOutput(indentationLevel, "%s", loop->collectionName);
    }
    
    generatorOutput(indentationLevel, ") ");
    
    if (loop->block != NULL) {
        generateScope(indentationLevel, loop->block);
    }
}

void generateLoop(const unsigned int indentationLevel, Loop* loop) {
    if (loop == NULL) {
        return;
    }
    
    switch (loop->type) {
        case WHILE_LOOP:
            generateWhileLoop(indentationLevel, loop);
            break;
        case FOR_LOOP:
            generateForLoop(indentationLevel, loop);
            break;
        default:
            break;
    }
} 