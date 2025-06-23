#include "StructureGenerator.h"
#include "ExpressionGenerator.h"
#include "ComparisonGenerator.h"
#include "ConditionalGenerator.h"
#include "VariableGenerator.h"
#include "AssignmentGenerator.h"
#include "LoopGenerator.h"
#include "Generator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>

void generateReturn(const unsigned int indentationLevel, Instruction* instruction) {
	generatorOutput(indentationLevel, "return ");
	if (instruction != NULL && instruction->returnInstruction != NULL) {
        generatorOutput(0, " ");
        generateInstruction(0, instruction->returnInstruction);
	} else {
        generatorOutput(0, ";\n");
	}
}

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
        case INSTRUCTION_CONDITIONAL:
            generateConditional(indentationLevel, instruction->conditional);
            break;
        case INSTRUCTION_LOOP:
            generateLoop(indentationLevel, instruction->loop);
            break;
		case INSTRUCTION_RETURN:
			generateReturn(indentationLevel, instruction);
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

void generateScope(const unsigned int indentationLevel, Block* block) {
    generatorOutput(indentationLevel, "{\n");
    generateBlock(indentationLevel, block);
    generatorOutput(indentationLevel, "}\n");
}

void generateProgram(const unsigned int indentationLevel, Program* program) {
    generateMainBlock(indentationLevel, program->block);
} 

