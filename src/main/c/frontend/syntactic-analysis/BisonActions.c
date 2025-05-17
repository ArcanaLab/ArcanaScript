#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
	logError(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Constant * ConstantSemanticAction(const void * value, ConstantType constantType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	switch (constantType) {
		case C_INT_TYPE:
			constant->intValue = *(int *)value;
			break;
		case C_CHAR_TYPE:
			constant->charValue = *(char *)value;
			break;
		case C_BOOLEAN_TYPE:
			constant->booleanValue = *(boolean *)value;
			break;
		case C_DOUBLE_TYPE:
			constant->doubleValue = *(double *)value;
			break;
		case C_FLOAT_TYPE:
			constant->floatValue = *(float *)value;
			break;
		case C_STRING_TYPE:
			constant->stringValue = *(char **)value;
			break;
	}

	constant->type = constantType;
	return constant;
}

Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * ComparatorExpressionSemanticAction(Factor * leftExpression, Factor * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftFactor = leftExpression;
	expression->rightFactor = rightExpression;
	expression->type = type;
	return expression;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Factor * ConstantFactorSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->constant = constant;
	factor->type = CONSTANT;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
}

VariableDeclaration * VariableDeclarationSemanticAction(char * name, VariableType type, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	VariableDeclaration * variableDeclaration = calloc(1, sizeof(VariableDeclaration));
	variableDeclaration->name = name;
	variableDeclaration->type = type;
	variableDeclaration->expression = expression;

	return variableDeclaration;
}
#pragma region "Program Semantic Actions"

Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->expression = expression;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;

}

Program * VariableProgramSemanticAction(CompilerState * compilerState, VariableDeclaration * varDeclaration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->variableDeclaration = varDeclaration;

	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;

}

Program * ConditionalProgramSemanticAction(CompilerState * compilerState, Conditional * conditional) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->conditional = conditional;

	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;

}
#pragma endregion

Conditional * IfConditionalSemanticAction(Expression * expression)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Conditional * conditional = calloc(1, sizeof(Conditional));
	conditional->expression = expression;
	conditional->ConditionalType = IF_TYPE;
	return conditional;
}



/**
 * Assignment operations.
 */
Program * AssignmentProgramSemanticAction(CompilerState * compilerState, AssignmentOperation * assignmentOperation) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->assignmentOperation = assignmentOperation;

	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}

AssignmentOperation * AssignmentDeclarationSemanticAction(
	VariableDeclaration * variableDeclaration,
	Expression * expression
) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	AssignmentOperation * assignmentOperation = calloc(1, sizeof(AssignmentOperation));
	assignmentOperation->variableDeclaration = variableDeclaration;
	assignmentOperation->assignmentOperator = ASSIGN_TYPE;
	assignmentOperation->expression = expression;

	return assignmentOperation;
}

AssignmentOperation * AssignmentOperatorSemanticAction(
	char * name,
	Expression * expression,
	AssignmentOperatorType assignmentOperatorType
) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	AssignmentOperation * assignmentOperation = calloc(1, sizeof(AssignmentOperation));
	assignmentOperation->name = name;
	assignmentOperation->assignmentOperator = assignmentOperatorType;
	assignmentOperation->expression = expression;

	return assignmentOperation;
}