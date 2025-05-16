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

VariableDeclarationNode * VariableDeclarationSemanticAction(char * identifier, TypeEnum type, ExpressionNode * expression) {	
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VariableDeclarationNode * variableDeclaration = calloc(1, sizeof(VariableDeclarationNode));
	variableDeclaration->identifier = identifier;
	variableDeclaration->type = type;
	variableDeclaration->expression = expression;
	return variableDeclaration;
}

ExpressionNode * ComparisonExpressionSemanticAction(ExpressionNode * leftExpression, ExpressionNode * rightExpression, TypeExpression comparisonOperator){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ExpressionNode * expression = calloc(1, sizeof(ExpressionNode));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = comparisonOperator;
	return expression;
}
ExpressionNode * ArithmeticExpressionSemanticAction(ExpressionNode * leftExpression, ExpressionNode * rightExpression, TypeExpression arithmeticOperator){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ExpressionNode * expression = calloc(1, sizeof(ExpressionNode));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = arithmeticOperator;
	return expression;
}

 /*============================== PROGRAM SEMANTIC ACTIONS ====================================*/


Program * ProgramSemanticAction(CompilerState * compilerState, ExpressionNode * expression){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->variableDeclaration = expression; 
	return program;

}

 /*============================== CONSTANT SEMANTIC ACTIONS ====================================*/

ConstantNode * ConstantExpressionSemanticAction(const char value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantNode * constant = calloc(1, sizeof(ConstantNode));
	return constant;
}

ConstantNode * CharConstantSemanticAction(const char value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantNode * constant = calloc(1, sizeof(ConstantNode));
	constant->charValue = value;
	constant->type = CHAR_TYPE;
	return constant;
}

ConstantNode * IntConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantNode * constant = calloc(1, sizeof(ConstantNode));
	constant->intValue = value;
	constant->type = INT_TYPE;
	return constant;
}

ConstantNode * FloatConstantSemanticAction(const float value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantNode * constant = calloc(1, sizeof(ConstantNode));
	constant->floatValue = value;
	constant->type = FLOAT_TYPE;
	return constant;
}
ConstantNode * DoubleConstantSemanticAction(const double value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantNode * constant = calloc(1, sizeof(ConstantNode));
	constant->doubleValue = value;
	constant->type = DOUBLE_TYPE;
	return constant;
}
ConstantNode * StringConstantSemanticAction(const char * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantNode * constant = calloc(1, sizeof(ConstantNode));
	constant->stringValue = value;
	constant->type = STRING_TYPE;
	return constant;
}
ConstantNode * BooleanConstantSemanticAction(const boolean value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantNode * constant = calloc(1, sizeof(ConstantNode));
	constant->booleanValue = value;
	constant->type = BOOLEAN_TYPE;
	return constant;
}
ConstantNode * LongConstantSemanticAction(const long value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantNode * constant = calloc(1, sizeof(ConstantNode));
	constant->longValue = value;
	constant->type = LONG_TYPE;
	return constant;
}
ConstantNode * ShortConstantSemanticAction(const short value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantNode * constant = calloc(1, sizeof(ConstantNode));
	constant->shortValue = value;
	constant->type = SHORT_TYPE;
	return constant;
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
