#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

// Initialize module's internal state.
void initializeAbstractSyntaxTreeModule();

// Shutdown module's internal state.
void shutdownAbstractSyntaxTreeModule();

// Typedefs
#pragma region Typedefs
typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;
typedef enum VariableType VariableType;
typedef enum ConstantType ConstantType;
typedef enum AssignmentOperatorType AssignmentOperatorType;
typedef enum InstructionType InstructionType;
typedef enum LoopType LoopType;
typedef enum ConditionalType ConditionalType;
typedef enum PrivacyType PrivacyType;

typedef struct Conditional Conditional;
typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef struct Loop Loop;

typedef struct PrivacyModifier PrivacyModifier;
typedef struct VariableDeclaration VariableDeclaration;
typedef struct AssignmentOperation AssignmentOperation;
typedef struct Instruction Instruction;
typedef struct Lambda Lambda;
typedef struct Class Class;
typedef struct Interface Interface;
typedef struct Import Import;

typedef struct FunctionCall FunctionCall;

typedef struct Object Object;
typedef struct Generic Generic;
#pragma endregion

	//  ------------ Lists ------------
#pragma region Lists
typedef struct Node Node;
typedef struct List List;
typedef void (*releaseDataFn)(void *);
#pragma endregion

	//  ------------ Specialized lists ------------
#pragma region Specialized Lists
typedef Node ExpressionNode;
typedef List ExpressionList;

typedef Node ImportStatementNode;
typedef List ImportList;

typedef Node VariableDeclarationNode;
typedef List VariableDeclarationList;

typedef Node PrivacyNode;
typedef List PrivacyList;

typedef Node InstructionNode;
typedef List Block;

typedef Node GenericListNode;
typedef List GenericList;

typedef Node ImplementationListNode;
typedef List ImplementationList;
#pragma endregion

/** ============== ENUMS ============== */
#pragma region Enums
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
};

enum ConditionalType {
	IF_TYPE,
	ELSE_IF_TYPE,
	ELSE_TYPE,
};

enum FactorType {
	VARIABLE_TYPE,
	INCREMENT_TYPE,
	DECREMENT_TYPE,
	CONSTANT,
	EXPRESSION,
};

enum PrivacyType {
	PRIVATE_A,
	PUBLIC_A,
	PROTECTED_A,
	CONST_A,
	STATIC_A,
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
	OBJECT
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
	INSTRUCTION_CLASS,
	INSTRUCTION_INTERFACE,
	INSTRUCTION_RETURN,
	INSTRUCTION_PASS,
};

enum LoopType {
	WHILE_LOOP,
	FOR_LOOP,
};
#pragma endregion

/** ============== STRUCTS ============== */
#pragma region Structs
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
		char * variable;
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
		struct {
			Factor * leftFactor;
			Factor * rightFactor;
		};
		FunctionCall * functionCall;
		Lambda * lambda;
	};
	ExpressionType type;
};

struct PrivacyModifier {
	PrivacyType type;
};

struct VariableDeclaration {
	char * name;
	Expression * expression;
	Object * object;
	VariableType type;
	PrivacyList * privacyModifierList;
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
		Class * class;
		Interface * interface;
		Instruction * returnInstruction;
	};

	InstructionType type;
};

struct Lambda {
	VariableDeclarationList * variableDeclarationList;
	Block * block;
};

struct Import{
	char * PathToFile;
};

struct Class {
	Object * object;
	Object * inherits;
	ImplementationList * implementationList;
	Block * block;
};

struct Interface {
	Object * object;
	ImplementationList * extends;
	Block * block;
};

struct FunctionCall {
	char * name;
	ExpressionList * expressionList;
};

struct Object {
	char * name;
	GenericList * genericList;
};

struct Generic {
	Object * object;
	Object * isObject;
};

struct Program {
	ImportList * importList;
	Block * block;
	Loop * loop;
};
#pragma endregion

	//  ------------ Lists ------------
#pragma region Lists
struct Node {
	void * data;
	Node * next;
};

struct List {
	Node * first;
	Node * last;
	int size;
};
#pragma endregion

/** ============== STRUCTS ============== */
#pragma region Function Declarations
// ================== [ Memory Release ] =======================
#pragma region MemoryRelease
/**
 * Releases the memory used by a constant.
 * @param constant Pointer to the constant to be released.
 */
void releaseConstant(Constant * constant);

/**
 * Releases the memory used by an expression and its components.
 * @param expression Pointer to the expression to be released.
 */
void releaseExpression(Expression * expression);

/**
 * Releases the memory used by a factor and its components.
 * @param factor Pointer to the factor to be released.
 */
void releaseFactor(Factor * factor);

/**
 * Releases the memory used by a name (string).
 * @param name Pointer to the name to be released.
 */
void releaseName(char * name);

/**
 * Releases the memory used by a variable declaration and its components.
 * @param variable Pointer to the variable declaration to be released.
 */
void releaseVariableDeclaration(VariableDeclaration * variable);

/**
 * Releases the memory used by an assignment operation and its components.
 * @param assignmentOperation Pointer to the assignment operation to be released.
 */
void releaseAssignmentOperation(AssignmentOperation * assignmentOperation);

/**
 * Releases the memory used by a conditional structure and its components.
 * @param conditional Pointer to the conditional structure to be released.
 */
void releaseConditional(Conditional * conditional); 

/**
 * Releases the memory used by a lambda expression and its components.
 * @param lambda Pointer to the lambda to be released.
 */
void releaseLambda(Lambda * lambda);

/**
 * Releases the memory used by a class and its components.
 * @param class Pointer to the class to be released.
 */
void releaseClass(Class * class);

/**
 * Releases the memory used by an instruction and its components.
 * @param instruction Pointer to the instruction to be released.
 */
void releaseInstruction(Instruction * instruction);

/**
 * Releases the memory used by a program and its components.
 * @param program Pointer to the program to be released.
 */
void releaseProgram(Program * program);

/**
 * Releases the memory used by a loop and its components.
 * @param loop Pointer to the loop to be released.
 */
void releaseLoop(Loop * loop);

/**
 * Releases the memory used by a function call and its components.
 * @param functionCall Pointer to the function call to be released.
 */
void releaseFunctionCall(FunctionCall * functionCall);

/**
 * Releases the memory used by an expression list and its components.
 * @param expressionList Pointer to the expression list to be released.
 */
void releaseExpressionList(ExpressionList * expressionList);

/**
 * Releases the memory used by a variable declaration list and its components.
 * @param variableDeclarationList Pointer to the variable declaration list to be released.
 */
void releaseVariableDeclarationList(VariableDeclarationList * variableDeclarationList);

/**
 * Releases the memory used by a privacy modifier.
 * @param modifier Pointer to the privacy modifier to be released.
 */
void releasePrivacyModifier(PrivacyModifier * modifier);

/**
 * Releases the memory used by a privacy modifier list and its components.
 * @param privacyList Pointer to the privacy modifier list to be released.
 */
void releasePrivacyList(PrivacyList * privacyList);

/**
 * Releases the memory used by a block of instructions and its components.
 * @param block Pointer to the block to be released.
 */
void releaseBlock(Block * block);

/**
 * Releases the memory used by a generic list and its components.
 * @param genericList Pointer to the generic list to be released.
 */
void releaseGenericList(GenericList * genericList);

/**
 * Releases the memory used by an object and its components.
 * @param object Pointer to the object to be released.
 */
void releaseObject(Object * object);

/**
 * Releases the memory used by a generic and its components.
 * @param generic Pointer to the generic to be released.
 */
void releaseGeneric(Generic * generic);

/**
 * Releases the memory used by an implementation list and its components.
 * @param implementationList Pointer to the implementation list to be released.
 */
void releaseImplementationList(ImplementationList * implementationList);

/**
 * Releases the memory used by an interface and its components.
 * @param interface Pointer to the interface to be released.
 */
void releaseInterface(Interface * interface);

/**
 * @brief Releases all resources associated with the given ImportList.
 *
 * This function frees any memory allocated for the ImportList and its contents.
 * After calling this function, the ImportList pointer should not be used unless reinitialized.
 *
 * @param importList Pointer to the ImportList to be released.
 */
void releaseImportList(ImportList * importList);

void releaseImport(Import * import_statement);
#pragma endregion
// =======================================================

#endif