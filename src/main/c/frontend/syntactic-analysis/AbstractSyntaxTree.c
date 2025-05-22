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
			case VARIABLE_TYPE:
				releaseName(factor->variable);
				break;
			case INCREMENT_TYPE:
			case DECREMENT_TYPE:
				releaseName(factor->variable);
				break;
		}
		free(factor);
	}
}


void releaseName(char * name) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(name == NULL) return;
	free(name);
}


void releaseVariableDeclaration(VariableDeclaration * variable) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(variable == NULL) return;

	releaseExpression(variable->expression);
	releaseName(variable->name);
	releasePrivacyList(variable->privacyModifierList);
	releaseObject(variable->object);
	free(variable);
}

void releasePrivacyModifier(PrivacyModifier * modifier) {
    if (modifier == NULL) return;
    free(modifier);
}
void releaseConditional(Conditional * conditional){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
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
		case INSTRUCTION_BLOCK:
			releaseBlock(instruction->block);
			break;
		case INSTRUCTION_LOOP:	
			releaseLoop(instruction->loop);
			break;
		case INSTRUCTION_CONDITIONAL:
			releaseConditional(instruction->conditional);
			break;
		case INSTRUCTION_CLASS:
			releaseClass(instruction->class);
			break;
		case INSTRUCTION_INTERFACE:
			releaseInterface(instruction->interface);
			break;
		case INSTRUCTION_RETURN:
			releaseInstruction(instruction->returnInstruction);
			break;
	}
	free(instruction);
}

void releaseLoop(Loop * loop) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
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
	releaseBlock(lambda->block);
	free(lambda);
}

void releaseClass(Class * class) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(class == NULL) return;

	releaseBlock(class->block);
	releaseObject(class->object);
	releaseObject(class->inherits);
	releaseImplementationList(class->implementationList);
	free(class);
}

void releaseImport(Import * import_statement)
{
	if (import_statement == NULL)return;

	free(import_statement->PathToFile);
	free(import_statement);

}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	releaseImportList(program->importList);
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

void releaseObject(Object * object) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(object == NULL) return;

	releaseName(object->name);
	releaseGenericList(object->genericList);
	free(object);
}

void releaseGeneric(Generic * generic) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(generic == NULL) return;

	releaseObject(generic->object);
	releaseObject(generic->isObject);
	free(generic);
}

void releaseInterface(Interface * interface) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(interface == NULL) return;

	releaseObject(interface->object);
	releaseImplementationList(interface->extends);
	releaseBlock(interface->block);
	free(interface);
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
void releaseImportList(ImportList * importList)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(importList,  (releaseDataFn) releaseImport);
}
// Expressions.
void releaseExpressionList(ExpressionList * expressionList){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(expressionList,  (releaseDataFn) releaseExpression);
}

// Variable declarations.
void releaseVariableDeclarationList(VariableDeclarationList * variableDeclarationList){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(variableDeclarationList, (releaseDataFn) releaseVariableDeclaration);
}

// Generics.
void releaseGenericList(GenericList * genericList){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(genericList, (releaseDataFn) releaseGeneric);
}

// Privacy modifiers.
void releasePrivacyList(PrivacyList * privacyList){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(privacyList, (releaseDataFn) releasePrivacyModifier);
}

// Implementation List.
void releaseImplementationList(ImplementationList * implementationList){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(implementationList, (releaseDataFn) releaseObject);
}

// Instructions & Blocks.
void releaseBlock(Block * block){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	return releaseList(block, (releaseDataFn) releaseInstruction);
}


// And that's all. We now can do it multiple times, with multiple uses, and keep it simple.