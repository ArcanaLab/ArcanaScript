#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */
typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;
typedef enum VariableType VariableType;
typedef enum ConstantType ConstantType;
typedef enum AssignmentOperatorType AssignmentOperatorType;
typedef enum InstructionType InstructionType;
typedef enum LoopType LoopType;
typedef enum ConditionalType ConditionalType;

typedef struct Conditional Conditional;
typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef struct Loop Loop;

typedef struct VariableDeclaration VariableDeclaration;
typedef struct AssignmentOperation AssignmentOperation;
typedef struct Instruction Instruction;
typedef struct Lambda Lambda;

typedef struct FunctionCall FunctionCall;


/** Lists */
typedef struct Node Node;
typedef struct List List;
typedef void (*releaseDataFn)(void *);

/**
 * Specialized lists.
 */

// Expressions.
typedef Node ExpressionNode;
typedef List ExpressionList;

// Variable declarations.
typedef Node VariableDeclarationNode;
typedef List VariableDeclarationList;

// Instructions & Blocks.
typedef Node InstructionNode;
typedef List Block;


/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/** ============== ENUMS ============== */
enum ExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION,
	LAMBDA,
	FUNCTION_CALL,
	LESS_TYPE,
	GREATER_TYPE,
	LESS_EQUAL_TYPE,
	GREATER_EQUAL_TYPE,
	EQUAL_EQUAL_TYPE,
	NOT_EQUAL_TYPE,
	VARIABLE_TYPE,
	INCREMENT_TYPE,
	DECREMENT_TYPE,
};
enum ConditionalType {
	IF_TYPE,
	ELSE_IF_TYPE,
	ELSE_TYPE,
};
enum FactorType {
	CONSTANT,
	EXPRESSION,
};

enum VariableType {
    V_INT,
	V_CHAR,
	V_BOOLEAN,
	V_STRING,
	V_DOUBLE,
	V_FLOAT,
	V_LONG,
	V_SHORT,
};

enum ConstantType {
	C_INT_TYPE,
	C_CHAR_TYPE,
	C_BOOLEAN_TYPE,
	C_STRING_TYPE,
	C_DOUBLE_TYPE,
	C_FLOAT_TYPE,	
};

enum AssignmentOperatorType {
	ASSIGN_TYPE,
	ADD_ASSIGN_TYPE,
	SUB_ASSIGN_TYPE,
	MUL_ASSIGN_TYPE,
};

enum InstructionType {
	INSTRUCTION_ASSIGNMENT,
	INSTRUCTION_VARIABLE_DECLARATION,
	INSTRUCTION_EXPRESSION,
	INSTRUCTION_BLOCK,
	INSTRUCTION_CONDITIONAL,
	INSTRUCTION_LOOP,
};

enum LoopType {
	WHILE_LOOP,
	FOR_LOOP,
};

/** ============== STRUCTS ============== */

struct Constant {
	union {
		int intValue;
		char charValue;
		boolean booleanValue;
		double doubleValue;
		float floatValue;
		char * stringValue;
	};
	ConstantType type;
};

struct Factor {
	union {
		Constant * constant;
		Expression * expression;
	};
	FactorType type;
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
		struct 
		{
			Factor * leftFactor;
			Factor * rightFactor;
		};
		FunctionCall * functionCall;
		Lambda * lambda;
		char * variable;
	};
	ExpressionType type;
};

struct VariableDeclaration {
	char * name;
	VariableType type;
	Expression * expression;
};

struct AssignmentOperation {
	union {
		char * name;
		VariableDeclaration * variableDeclaration;
	};
	Expression * expression;
	AssignmentOperatorType assignmentOperator;
};

struct Loop {
	Expression * expression;
	LoopType type;
	char * itemName;
	char * collectionName;
	Block * block;
};
struct Conditional {
	Expression * expression;
	Conditional * nextConditional;
	ConditionalType ConditionalType;
	Block * block;
};

struct Instruction {
	union {
		AssignmentOperation * assignment;
		VariableDeclaration * variableDeclaration;
		Expression * expression;
		Block * block;
		Loop * loop;
		Conditional * conditional;
	};

	InstructionType type;
};

struct Lambda {
	VariableDeclarationList * variableDeclarationList;
	Instruction * instruction;
};

struct FunctionCall {
	char * name;
	ExpressionList * expressionList;
};

struct Program {
	Block * block;
	Loop * loop;
};


/***
 * LISTS
 */
struct Node {
	void * data;
	Node * next;
};

struct List {
	Node * first;
	Node * last;
	int size;
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseName(char * name);
void releaseVariableDeclaration(VariableDeclaration * variable);
void releaseAssignmentOperation(AssignmentOperation * assignmentOperation);
void releaseConditional(Conditional * conditional); 
void releaseLambda(Lambda * lambda);

void releaseInstruction(Instruction * instruction);

void releaseProgram(Program * program);
void releaseLoop(Loop * loop);

void releaseFunctionCall(FunctionCall * functionCall);

void releaseExpressionList(ExpressionList * expressionList);
void releaseVariableDeclarationList(VariableDeclarationList * variableDeclarationList);
void releaseBlock(Block * block);
#endif
