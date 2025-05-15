#include "BisonActions.h"

/*============================== Structures ====================================*/


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
}
Program * ExpressionProgramSemanticAction(CompilerState * compilerState, ExpressionNode * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->variableDeclaration = expression; // HARD CODED
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

VariableDeclarationNode * VariableSemanticAction(char * name, TypeEnum * type, ExpressionNode * expression) {	
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VariableDeclarationNode * variableDeclaration = calloc(1, sizeof(VariableDeclarationNode));
	variableDeclaration->name = name;
	variableDeclaration->type = type;
	variableDeclaration->expression = expression;
	return variableDeclaration;
}

// /* PUBLIC FUNCTIONS */

// Constant * IntegerConstantSemanticAction(const int value) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Constant * constant = calloc(1, sizeof(Constant));
// 	constant->value = value;
// 	return constant;
// }
// /*============================== OPERATOR SEMANTIC ACTIONS ====================================*/
// ExpressionNode * makeComparisonNode(ExpressionNode * leftExpression, ExpressionNode * rightExpression, TypeExpression type) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Expression * expression = calloc(1, sizeof(Expression));
// 	expression->leftExpression = leftExpression;
// 	expression->rightExpression = rightExpression;
// 	expression->type = type;
// 	return expression;
// }

// ExpressionNode * ArithmeticExpressionSemanticAction(ExpressionNode * leftExpression, ExpressionNode * rightExpression, TypeExpression type) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	ExpressionNode * expression = calloc(1, sizeof(Expression));
// 	expression->leftExpression = leftExpression;
// 	expression->rightExpression = rightExpression;
// 	expression->type = type;
// 	return expression;
// }

// Expression * FactorExpressionSemanticAction(Factor * factor) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Expression * expression = calloc(1, sizeof(Expression));
// 	expression->factor = factor;
// 	expression->type = FACTOR;
// 	return expression;
// }	

// Factor * ConstantFactorSemanticAction(Constant * constant) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Factor * factor = calloc(1, sizeof(Factor));
// 	factor->constant = constant;
// 	factor->type = CONSTANT;
// 	return factor;
// }

// Factor * ExpressionFactorSemanticAction(Expression * expression) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Factor * factor = calloc(1, sizeof(Factor));
// 	factor->expression = expression;
// 	factor->type = EXPRESSION;
// 	return factor;
// }

