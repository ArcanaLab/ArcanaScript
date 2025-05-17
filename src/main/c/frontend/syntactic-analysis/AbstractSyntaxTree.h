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

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;

typedef struct VariableDeclaration VariableDeclaration;
typedef struct AssignmentOperation AssignmentOperation;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/** ============== ENUMS ============== */
enum ExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION
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

struct Program {
	Expression * expression;
	VariableDeclaration * variableDeclaration;
	AssignmentOperation * assignmentOperation;
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
void releaseProgram(Program * program);

#endif
