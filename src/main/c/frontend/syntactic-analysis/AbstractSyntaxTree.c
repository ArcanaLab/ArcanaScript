#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */
void releaseConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant == NULL) return;

	if(constant->type == C_STRING_TYPE) free(constant->stringValue);
	free(constant);
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(expression == NULL) return;
	switch (expression->type) {
		case ADDITION:
		case DIVISION:
		case MULTIPLICATION:
		case SUBTRACTION:
			releaseExpression(expression->leftExpression);
			releaseExpression(expression->rightExpression);
			break;
		case FACTOR:
			releaseFactor(expression->factor);
			break;
	}
	free(expression);
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				releaseConstant(factor->constant);
				break;
			case EXPRESSION:
				releaseExpression(factor->expression);
				break;
		}
		free(factor);
	}
}

void releaseName(char * name) {
	if(name == NULL) return;
	logError(_logger, "Executing destructor: %s", __FUNCTION__);
	free(name);
}

void releaseVariableDeclaration(VariableDeclaration * variable) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(variable == NULL) return;

	releaseExpression(variable->expression);
	releaseName(variable->name);
	free(variable);
}

void releaseAssignmentOperation(AssignmentOperation * assignmentOperation) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (assignmentOperation == NULL) return;

	releaseName(assignmentOperation->name);
	releaseExpression(assignmentOperation->expression);
	free(assignmentOperation);
}

void releaseInstruction(Instruction * instruction) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(instruction == NULL) return;

	switch (instruction->type) {
		case INSTRUCTION_ASSIGNMENT:
			releaseAssignmentOperation(instruction->assignment);
			break;
		case INSTRUCTION_VARIABLE_DECLARATION:
			releaseVariableDeclaration(instruction->variableDeclaration);
			break;
		case INSTRUCTION_EXPRESSION:
			releaseExpression(instruction->expression);
			break;
	}
	free(instruction);
}

void releaseInstructionNode(InstructionNode * instructionNode) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(instructionNode == NULL) return;

	releaseInstruction(instructionNode->instruction);
	free(instructionNode);
}

void releaseInstructionList(InstructionNode * instructionNode) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(instructionNode == NULL) return;
	
	InstructionNode * currentInstructionNode = instructionNode;
	while(currentInstructionNode != NULL) {
		InstructionNode * nextInstructionNode = currentInstructionNode->nextInstructionNode;
		releaseInstructionNode(currentInstructionNode);
		currentInstructionNode = nextInstructionNode;
	}
}

void releaseBlock(Block * block) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(block == NULL) return;

	releaseInstructionList(block->firstInstructionNode);
	free(block);
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(program == NULL) return;

	releaseBlock(program->block);
	free(program);
}
