#include "BisonActions.h"

/* MODULE INTERNAL STATE */
static Logger * _logger = NULL;
static int _abort_parse = 0;

int hasAborted(void) {
	return _abort_parse;
}

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

VariableDeclaration * VariableDeclarationSemanticAction(char * name, VariableType type, Expression * expression, Object * object, PrivacyList * privacyModifierList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	VariableDeclaration * variableDeclaration = calloc(1, sizeof(VariableDeclaration));
	variableDeclaration->name = name;
	variableDeclaration->type = type;
	variableDeclaration->expression = expression;
	variableDeclaration->privacyModifierList = privacyModifierList;
	variableDeclaration->object = object;
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

void ValidateContext(ContextStackType contextType, const char * errorMessage){
	if (!inContext(contextType)) {
		logError(_logger, errorMessage);
		yyerror(errorMessage);
		_abort_parse = 1;
	}
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

		case INSTRUCTION_CLASS:
			instruction->class = value;
			break;

		case INSTRUCTION_INTERFACE:
			instruction->interface = value;
			break;

		case INSTRUCTION_RETURN:
			instruction->returnInstruction = value;
			ValidateContext(LAMBDA_CONTEXT, "Return statements are not allowed outside functions.");
			break;

		case INSTRUCTION_PASS:
			ValidateContext(LOOP_CONTEXT, "Pass statements are not allowed outside loops.");
			break;
	}

	return instruction;
}

Lambda * LambdaSemanticAction(VariableDeclarationList * variableDeclarationList, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Lambda * lambda = calloc(1, sizeof(Lambda));
	lambda->variableDeclarationList = variableDeclarationList;
	lambda->block = block;
	return lambda;
}

Expression * LambdaExpressionSemanticAction(Lambda * lambda) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->lambda = lambda;
	expression->type = LAMBDA;
	return expression;
}

Program * BlockProgramSemanticAction(CompilerState * compilerState,ImportList * importList, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->importList = importList;
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

Class * ClassSemanticAction(Object * object, Object * inherits, ImplementationList * implementationList, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Class * class = calloc(1, sizeof(Class));
	class->object = object;
	class->inherits = inherits;
	class->implementationList = implementationList;
	class->block = block;
	return class;
}

FunctionCall * FunctionCallSemanticAction(char * name, ExpressionList * expressionList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FunctionCall * functionCall = calloc(1, sizeof(FunctionCall));
	functionCall->name = name;
	functionCall->expressionList = expressionList;
	return functionCall;
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

PrivacyModifier * PrivacyModifierSemanticAction(PrivacyType privacyType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PrivacyModifier * privacyModifier = calloc(1, sizeof(PrivacyModifier));
	privacyModifier->type = privacyType;
	return privacyModifier;
}

Expression * UnaryExpressionSemanticAction(char * name,  ExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->variable = name;
	expression->type = type;
	return expression;
}

Object * ObjectSemanticAction(char * name, GenericList * genericList){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Object * object = calloc(1, sizeof(Object));
	object->name = name;
	object->genericList = genericList;

	return object;
}

Generic * GenericSemanticAction(Object * object, Object * isObject){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Generic * generic = calloc(1, sizeof(Generic));
	generic->object = object;
	generic->isObject = isObject;
	return generic;
}

// ===== Interfaces =====
Interface * InterfaceSemanticAction(Object * object, ImplementationList * extends, Block * block){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Interface * interface = calloc(1, sizeof(Interface));
	interface->object = object;
	interface->extends = extends;
	interface->block = block;
	return interface;
}

// ===== Imports =====
Import * ImportSemanticAction(char * path)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Import * import_statement = calloc(1,sizeof(Import));
	import_statement->PathToFile = path;
	return import_statement;
}


/**
 * ============== LIST ==============
 */

// ======= General purpose =======
/**
 * @param data the data to insert into the list
 */
List * CreateListSemanticAction(void * data){
	List * list = calloc(1, sizeof(List));
	list->first = NULL;
	list->last = NULL;
	list->size = 0;

	if(data == NULL) return list;
	return ListSemanticAction(list, data);	
}

/**
 * @param list The List to insert data. If `NULL` it creates a new list with the data.
 * @param data the data to insert into the list. If `NULL`, there's nothing to insert and it returns the list as is.
 * 
 * @return A list with the data to be inserted. If `data = NULL` it returns the list. If both are `NULL` it returns a new empty list.
 */
List * ListSemanticAction(List * list, void * data){
	if(list == NULL) return CreateListSemanticAction(data);
	if(data == NULL) return list;

	Node * node = calloc(1, sizeof(Node));

	node->data = data; // YOUR DATA

	Node * last = list->last;

	if(last != NULL) last->next = node;
	else if(list->first == NULL) list->first = node;

	list->last = node;
	list->size += 1;

	return list;
}

// ======= Specialized lists ======= //

// Expressions.
ExpressionList * ExpressionListSemanticAction(ExpressionList * expressionList, 	Expression * expression){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return ListSemanticAction(expressionList, expression);
}

// Variable declarations.
VariableDeclarationList * VariableDeclarationListSemanticAction(VariableDeclarationList * variableDeclarationList, VariableDeclaration * variableDeclaration){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return ListSemanticAction(variableDeclarationList, variableDeclaration);
}
// Privacy
PrivacyList * PrivacyListSemanticAction(PrivacyList * privacyList, PrivacyModifier * modifier){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return ListSemanticAction(privacyList, modifier);
}

// Generic List
GenericList * GenericListSemanticAction(GenericList * genericList, Generic * generic){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return ListSemanticAction(genericList, generic);
}

// Implementation List.
ImplementationList * ImplementationListSemanticAction(ImplementationList * implementationList, Object * object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return ListSemanticAction(implementationList, object);
}

// Instructions & Blocks.
Block * BlockSemanticAction(Block * block, Instruction * instruction){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return ListSemanticAction(block, instruction);
}
ImportList * ImportListSemanticAction(ImportList * importList, Import * importStatement)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return ListSemanticAction(importList,importStatement);
}