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

 //@TODO: Destructors
	
	typedef struct Program Program;
	typedef struct ConstantNode ConstantNode;
	typedef struct VariableDeclarationNode VariableDeclarationNode;
	typedef struct ExpressionNode ExpressionNode;

	
 typedef enum TypeExpression {
	ADDITION,
 	DIVISION,
	MULTIPLICATION,
	SUBTRACTION,
	EQUAL,
	GREATER,
	GREATER_EQUAL,
	LESS,
	LESS_EQUAL,
	NOT_EQUAL
} TypeExpression;

typedef enum TypeEnum {
    CHAR_TYPE,
    INT_TYPE,
    DOUBLE_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
    BOOLEAN_TYPE,
    LONG_TYPE,
    SHORT_TYPE
} TypeEnum;



struct ConstantNode {
	union {
		char charValue;
		int intValue;
		double doubleValue;
		float floatValue;
		char * stringValue;
		boolean booleanValue;
		long longValue;
		short shortValue;
	};
	TypeEnum type;
};


struct Program {
	VariableDeclarationNode * variableDeclaration;

};

struct VariableDeclarationNode {
	char * identifier;
	TypeEnum type;
	ExpressionNode * expression;
};

struct ExpressionNode {
	union {
		struct {
			ExpressionNode * leftExpression;
			ExpressionNode * rightExpression;
		};
	};
	TypeExpression type;
};



/**
 * Node types for the Abstract Syntax Tree (AST).
 */
// enum ExpressionType {
// 	DIVISION,
// 	FACTOR,
// 	MULTIPLICATION,
// 	SUBTRACTION
// };

// enum FactorType {
// 	CONSTANT,
// 	EXPRESSION
// };

// struct Constant {
// 	int value;
// };

// struct Factor {
// 	union {
// 		ConstantNode * constant;
// 		ExpressionType * expression;
// 	};
// 	FactorType type;
// };

// struct Expression {
// 	union {
// 		Factor * factor;
// 		struct {
// 			Expression * leftExpression;
// 			Expression * rightExpression;
// 		};
// 	};
// 	TypeExpression type;
// };

// struct Program {
// 	Expression * expression;
// };


/**
 * Node recursive destructors.
 */

void releaseConstant(ConstantNode * constant);
void releaseExpression(ExpressionNode * expression);
//void releaseFactor(Factor * factor);
void releaseProgram(Program * program);

#endif
