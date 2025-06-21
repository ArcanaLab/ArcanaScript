#include "StructureGenerator.h"
#include "ExpressionGenerator.h"
#include "Generator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

void generateInstruction(const unsigned int indentationLevel, Instruction* instruction) {
    switch (instruction->type) {
        case INSTRUCTION_EXPRESSION:
            generateExpression(indentationLevel + 1, instruction->expression);
            break;
		default:
			exit(140);
			break;
    }
}

void generateBlock(const unsigned int indentationLevel, Block* block) {
    InstructionNode* currentInstruction = block->first;
    while (currentInstruction != NULL) {
        generateInstruction(indentationLevel + 1, (Instruction*)currentInstruction->data);
        currentInstruction = currentInstruction->next;
    }
}

void generateProgram(const unsigned int indentationLevel, Program* program) {
    generateBlock(indentationLevel, program->block);
} 