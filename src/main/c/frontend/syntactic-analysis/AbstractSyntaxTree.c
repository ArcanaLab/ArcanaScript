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

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(program == NULL) return;

	releaseVariableDeclaration(program->variableDeclaration);
	releaseAssignmentOperation(program->assignmentOperation);
	releaseExpression(program->expression);
	releaseLoop(program->loop);
	free(program);
}

void releaseAssignmentOperation(AssignmentOperation * assignmentOperation) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (assignmentOperation == NULL) return;

	// releaseVariableDeclaration(assignmentOperation->variableDeclaration);
	releaseName(assignmentOperation->name);
	releaseExpression(assignmentOperation->expression);
	free(assignmentOperation);
}

void releaseLoop(Loop * loop) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (loop == NULL) return;
	releaseExpression(loop->expression);
	free(loop);
}