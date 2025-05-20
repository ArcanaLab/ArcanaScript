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
		case LESS_TYPE:
		case GREATER_TYPE:
		case LESS_EQUAL_TYPE:
		case GREATER_EQUAL_TYPE:
		case EQUAL_EQUAL_TYPE:
		case NOT_EQUAL_TYPE:
			releaseFactor(expression->leftFactor);
			releaseFactor(expression->rightFactor);
			break;
		case FUNCTION_CALL:
			releaseFunctionCall(expression->functionCall);
			break;
		case LAMBDA:
			releaseLambda(expression->lambda);
			break;
		case VARIABLE_TYPE:
			releaseName(expression->variable);
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
	if (variable->privacyModifierList != NULL) {
		releaseList(variable->privacyModifierList,releasePrivacyModifier);
	}
	free(variable);
}
void releasePrivacyModifier(PrivacyModifier * modifier) {
    if (modifier == NULL) return;
    free(modifier);
}
void releaseConditional(Conditional * conditional){
	logError(_logger, "Executing destructor: %s", __FUNCTION__);
	if (!conditional) return;

	releaseExpression(conditional->expression);

	if (conditional->nextConditional)
		releaseConditional(conditional->nextConditional);
	releaseBlock(conditional->block);
	free(conditional);
}

void releaseAssignmentOperation(AssignmentOperation * assignmentOperation) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (assignmentOperation == NULL) return;

	releaseName(assignmentOperation->name);
	releaseExpression(assignmentOperation->expression);
	free(assignmentOperation);
}

void releaseInstruction(Instruction * instruction) {
	logError(_logger, "Executing destructor: %s", __FUNCTION__);
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
		case INSTRUCTION_BLOCK:
			releaseBlock(instruction->block);
			break;
		case INSTRUCTION_LOOP:	
			releaseLoop(instruction->loop);
			break;
		case INSTRUCTION_CONDITIONAL:
			releaseConditional(instruction->conditional);
			break;
	}
	free(instruction);
}

void releaseLoop(Loop * loop) {
	logError(_logger, "Executing destructor: %s", __FUNCTION__);
	if (loop == NULL) return;
	releaseExpression(loop->expression);
	releaseBlock(loop->block);
	releaseName(loop->itemName);
	releaseName(loop->collectionName);
	free(loop);
}

void releaseLambda(Lambda * lambda) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(lambda == NULL) return;

	releaseVariableDeclarationList(lambda->variableDeclarationList);
	releaseInstruction(lambda->instruction);
	free(lambda);
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(program == NULL) return;

	releaseBlock(program->block);
	free(program);
}

void releaseFunctionCall(FunctionCall * functionCall) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(functionCall == NULL) return;

	releaseExpressionList(functionCall->expressionList);
	releaseName(functionCall->name);
	free(functionCall);
}

/** 
 * 
 * How to release Nodes List in General.
 */


/**
 * ============== LISTS ==============
 */
// ======= General purpose ======= //

/**
 * Function is used to release a List.
 * 
 * @param list The List to release the memory.
 * @param release_fun The given function to release the data inside of a Node.
 */
void releaseList(List * list, releaseDataFn release_fun) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(list == NULL) return;

	Node * current = list->first;
	while(current != NULL){
		Node * next = current->next;
		release_fun(current->data);
		free(current);
		current = next;
	}
	free(list);
}

// ======= Specialized lists ======= //

// Expressions.
void releaseExpressionList(ExpressionList * expressionList){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(expressionList, releaseExpression);
}

// Variable declarations.
void releaseVariableDeclarationList(VariableDeclarationList * variableDeclarationList){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(variableDeclarationList, releaseVariableDeclaration);
}
void releasePrivacyList(PrivacyList * privacyList){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(privacyList, releasePrivacyModifier);
}
// Instructions & Blocks.
void releaseBlock(Block * block){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(block, releaseInstruction);
}
// And that's all. We now can do it multiple times, with multiple uses, and keep it simple.