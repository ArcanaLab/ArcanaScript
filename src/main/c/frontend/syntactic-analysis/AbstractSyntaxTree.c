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
	logDebugging(_logger, "Freeing Constant at %p with value at %p", constant, constant->value);
	if (constant != NULL) {
		free(constant->value);
		free(constant);
	}
}

void releaseExpression(Expression * expression) {
	logError(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
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
			case GREATER_TYPE:
			case GREATER_EQUAL_TYPE:
			case LESS_TYPE:
			case LESS_EQUAL_TYPE:
			case EQUAL_EQUAL_TYPE:
			case NOT_EQUAL_TYPE:
				releaseFactor(expression->leftFactor);
				releaseFactor(expression->rightFactor);
				break;
		}
		free(expression);
	}
}

void releaseFactor(Factor * factor) {
	logError(_logger, "Executing destructor: %s", __FUNCTION__);
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

void releaseVariableDeclaration(VariableDeclaration * variable) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(variable == NULL) return;

	if(variable->expression != NULL) releaseExpression(variable->expression);
	free(variable->name); // It must be done since it's name is allocated when strdup is done.
	free(variable);
}

void releaseConditional(Conditional * conditional){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (!conditional) return;

	releaseExpression(conditional->expression);
	free(conditional);
}


void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (!program) return;

	releaseExpression(program->expression);
	releaseVariableDeclaration(program->variableDeclaration);
	releaseConditional(program->conditional);
	free(program);
}

