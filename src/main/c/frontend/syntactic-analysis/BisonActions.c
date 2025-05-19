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

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Expression * ComparatorExpressionSemanticAction(Factor * leftFactor, Factor * rightFactor, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftFactor = leftFactor;
	expression->rightFactor = rightFactor;
	expression->type = type;
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

Loop * LoopSemanticAction(Expression * expression, LoopType type, Block * block, char * itemName, char * collectionName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Loop * loop = calloc(1, sizeof(Loop));
	loop->type = type;
	loop->expression = expression;
	loop->block = block;
	loop->itemName = itemName;
	loop->collectionName = collectionName;
	return loop;
}
Program * LoopProgramSemanticAction(CompilerState * compilerState, Loop * loop) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->loop = loop;

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
/**
 * Assignment operations.
 */
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

Conditional * ConditionalSemanticAction(Expression * expression, ConditionalType conditionalType,Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Conditional * conditional = calloc(1, sizeof(Conditional));
	conditional->expression = expression;
	conditional->nextConditional = NULL;
	conditional->block = block;
	conditional->ConditionalType = conditionalType;

	return conditional;
}

Instruction * InstructionSemanticAction(void * value, InstructionType instructionType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Instruction * instruction = calloc(1, sizeof(Instruction));
	instruction->type = instructionType;
	
	switch (instructionType) {
		case INSTRUCTION_ASSIGNMENT:
			instruction->assignment = value;
			break;
		case INSTRUCTION_VARIABLE_DECLARATION:
			instruction->variableDeclaration = value;
			break;
		case INSTRUCTION_EXPRESSION:
			instruction->expression = value;
			break;
		case INSTRUCTION_BLOCK:
			instruction->block = value;
			break;
		case INSTRUCTION_LOOP:
			instruction->loop = value;
			break;
		case INSTRUCTION_CONDITIONAL:
			instruction->conditional = value;
			break;
	}

	return instruction;
}

Block * CreateBlockSemanticAction(Instruction * instruction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Block * block = calloc(1, sizeof(Block));
	block->firstInstructionNode = calloc(1, sizeof(InstructionNode));
	block->firstInstructionNode->instruction = instruction;
	block->lastInstructionNode = block->firstInstructionNode;
	return block;
}

Block * AppendInstructionSemanticAction(Block * block, Instruction * instruction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InstructionNode * newInstructionNode = calloc(1, sizeof(InstructionNode));
	newInstructionNode->instruction = instruction;
	block->lastInstructionNode->nextInstructionNode = newInstructionNode;
	block->lastInstructionNode = newInstructionNode;
	return block;
}

Lambda * LambdaSemanticAction(VariableDeclarationList * variableDeclarationList,  Instruction * instruction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Lambda * lambda = calloc(1, sizeof(Lambda));
	lambda->variableDeclarationList = variableDeclarationList;
	lambda->instruction = instruction;
	return lambda;
}

Expression * LambdaExpressionSemanticAction(Lambda * lambda) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->lambda = lambda;
	expression->type = LAMBDA;
	return expression;
}

VariableDeclarationList * CreateVariableDeclarationListSemanticAction(VariableDeclaration * variableDeclaration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VariableDeclarationList * variableDeclarationList = calloc(1, sizeof(VariableDeclarationList));
	variableDeclarationList->firstNode = calloc(1, sizeof(VariableDeclarationNode));
	variableDeclarationList->firstNode->variableDeclaration = variableDeclaration;
	variableDeclarationList->lastNode = variableDeclarationList->firstNode;
	return variableDeclarationList;
}

VariableDeclarationList * AppendVariableDeclarationSemanticAction(VariableDeclarationList * variableDeclarationList, VariableDeclaration * variableDeclaration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VariableDeclarationNode * newVariableDeclarationNode = calloc(1, sizeof(VariableDeclarationNode));
	newVariableDeclarationNode->variableDeclaration = variableDeclaration;
	variableDeclarationList->lastNode->next = newVariableDeclarationNode;
	variableDeclarationList->lastNode = newVariableDeclarationNode;
	return variableDeclarationList;
}
Program * BlockProgramSemanticAction(CompilerState * compilerState, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->block = block;
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



FunctionCall * FunctionCallSemanticAction(char * name, ExpressionList * expressionList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FunctionCall * functionCall = calloc(1, sizeof(FunctionCall));
	functionCall->name = name;
	functionCall->expressionList = expressionList;
	return functionCall;
}

ExpressionList * CreateExpressionListSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ExpressionList * expressionList = calloc(1, sizeof(ExpressionList));
	expressionList->firstNode = calloc(1, sizeof(ExpressionNode));
	expressionList->firstNode->expression = expression;
	expressionList->lastNode = expressionList->firstNode;
	return expressionList;
}

ExpressionList * AppendExpressionSemanticAction(ExpressionList * expressionList, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ExpressionNode * newExpressionNode = calloc(1, sizeof(ExpressionNode));
	newExpressionNode->expression = expression;
	expressionList->lastNode->next = newExpressionNode;
	expressionList->lastNode = newExpressionNode;
	return expressionList;
}

Expression * FunctionCallExpressionSemanticAction(FunctionCall * functionCall) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->functionCall = functionCall;
	expression->type = FUNCTION_CALL;
	return expression;
}

Expression * VariableExpressionSemanticAction(char * variable){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->variable = variable;
	expression->type = VARIABLE_TYPE;
	return expression;
}
