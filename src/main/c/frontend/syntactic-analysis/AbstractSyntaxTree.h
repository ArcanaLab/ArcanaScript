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
typedef enum ConditionalType ConditionalType;

typedef struct Conditional Conditional;
typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;


typedef struct VariableDeclaration VariableDeclaration;

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
	GREATER_TYPE,
	LESS_TYPE,
	GREATER_EQUAL_TYPE,
	LESS_EQUAL_TYPE,
	EQUAL_EQUAL_TYPE,
	NOT_EQUAL_TYPE,
};

enum ConditionalType {
	IF_TYPE,
	ELSE_TYPE,
	ELIF_TYPE,
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

/** ============== STRUCTS ============== */

struct Constant {
	void * value;
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
	};
	ExpressionType type;
};

struct VariableDeclaration {
	char * name;
	VariableType type;
	Expression * expression;
};

struct Program {
	Expression * expression;
	VariableDeclaration * variableDeclaration;
	Conditional * conditional;
};

/*
	if (exp)
	{
	}
	else if (exp)
	{
	}
	else
	{
	}
*/

struct Conditional {
	Expression * expression;
	ConditionalType ConditionalType;
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseVariableDeclaration(VariableDeclaration * variable);
void releaseProgram(Program * program);
void releaseConditional(Conditional * conditional);

#endif
