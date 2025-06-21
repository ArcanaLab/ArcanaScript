#ifndef STRUCTURE_GENERATOR_HEADER
#define STRUCTURE_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

/**
 * Generates code for a program.
 * @param indentationLevel The current indentation level
 * @param program The program to generate code for
 */
void generateProgram(const unsigned int indentationLevel, Program* program);

/**
 * Generates code for the main program block (top-level instructions without extra indentation).
 * @param indentationLevel The current indentation level
 * @param block The block to generate code for
 */
void generateMainBlock(const unsigned int indentationLevel, Block* block);

/**
 * Generates code for a block of instructions.
 * @param indentationLevel The current indentation level
 * @param block The block to generate code for
 */
void generateBlock(const unsigned int indentationLevel, Block* block);

/**
 * Generates code for an instruction.
 * @param indentationLevel The current indentation level
 * @param instruction The instruction to generate code for
 */
void generateInstruction(const unsigned int indentationLevel, Instruction* instruction);

#endif 