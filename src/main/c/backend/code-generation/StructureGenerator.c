#include "StructureGenerator.h"
#include "ExpressionGenerator.h"
#include "VariableGenerator.h"
#include "AssignmentGenerator.h"
#include "Generator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

void generateInstruction(const unsigned int indentationLevel, Instruction* instruction) {
    switch (instruction->type) {
        case INSTRUCTION_EXPRESSION:
            generateExpression(indentationLevel, instruction->expression);
            generatorOutput(indentationLevel, ";\n");
            break;
        case INSTRUCTION_VARIABLE_DECLARATION:
            generateVariableDeclaration(indentationLevel, instruction->variableDeclaration);
            break;
        case INSTRUCTION_ASSIGNMENT:
            generateSimpleAssignment(indentationLevel, instruction->assignment);
            break;
        case INSTRUCTION_BLOCK:
            generateBlock(indentationLevel + 1, instruction->block);
            break;
		default:
			exit(140);
			break;
    }
}

void generateMainBlock(const unsigned int indentationLevel, Block* block) {
    InstructionNode* currentInstruction = block->first;
    while (currentInstruction != NULL) {
        generateInstruction(indentationLevel, (Instruction*)currentInstruction->data);
        currentInstruction = currentInstruction->next;
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
    generateMainBlock(indentationLevel, program->block);
} 