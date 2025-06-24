#include "StructureGenerator.h"
#include "ExpressionGenerator.h"
#include "ComparisonGenerator.h"
#include "ConditionalGenerator.h"
#include "VariableGenerator.h"
#include "AssignmentGenerator.h"
#include "LoopGenerator.h"
#include "ClassGenerator.h"
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

void generateConstructor(const unsigned int indentationLevel, Lambda* constructor) {
	if (constructor == NULL) {
		return;
	}
	
	generatorOutput(indentationLevel, "(");
	if (constructor->variableDeclarationList != NULL) {
		VariableDeclarationNode* current = constructor->variableDeclarationList->first;
		bool first = true;
		while (current != NULL) {
			VariableDeclaration* param = (VariableDeclaration*)current->data;
			if (!first) {
				generatorOutput(0, ", ");
			}
			if (param->type == OBJECT && param->object != NULL) {
				generatorOutput(0, "%s %s", param->object->name, param->name);
			} else {
				char* typeString = VariableTypeToString(param->type);
				generatorOutput(0, "%s %s", typeString, param->name);
			}
			first = false;
			current = current->next;
		}
	}
	generatorOutput(0, ") ");
	
	if (constructor->block != NULL) {
		generateScope(indentationLevel, constructor->block);
	} else {
		generatorOutput(indentationLevel, "{\n");
		generatorOutput(indentationLevel, "}\n");
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
		case INSTRUCTION_CLASS:
			generateClass(indentationLevel, instruction->class);
			break;
		case INSTRUCTION_INTERFACE:
			generateInterface(indentationLevel, instruction->interface);
			break;
		case INSTRUCTION_CONSTRUCTOR:
			generateConstructor(indentationLevel, instruction->constructor);
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

char* generatePath(char* path) {
    char* str = strdup(path);
    if (!str) return NULL;
    
    char* last_slash = strrchr(str, '/');
    if (last_slash) {
        *last_slash = '\0';
    }
    
    for (char* p = str; *p; ++p) {
        if (*p == '/') {
            *p = '.';
        }
    }
    return str;
}

void generateProgram(const unsigned int indentationLevel, Program* program) {
    if (program != NULL && program->importList != NULL) {
        Node* current = program->importList->first;
        while (current != NULL) {
            Import* importStatement = (Import*)current->data;
            if (importStatement != NULL && importStatement->PathToFile != NULL) {
                char* path = generatePath(importStatement->PathToFile);
                generatorOutput(0, "import %s;\n", path);
                free(path);
            }
            current = current->next;
        }
    }
    generateMainBlock(indentationLevel, program->block);
} 

