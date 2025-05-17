%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */
	int c_integer;
	char c_character;
	double c_double;
	float c_float;
	boolean c_boolean;
	char * c_string;
	
	Token token;
	
	/** Non-terminals. */
	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;


	VariableDeclaration * variableDeclaration;
	VariableType varType;

	AssignmentOperation * assignmentOperation;
	AssignmentOperatorType assignmentOperatorType;
	char * name;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseVariableDeclaration($$); } <variableDeclaration>
%destructor { releaseAssignmentOperation($$); } <assignmentOperation>

/** ============== TERMINALS. ============== */
%token <name> NAME
%token <varType> TYPE

/** ===== Factor Types ===== */
%token <c_integer> C_INTEGER
%token <c_character> C_CHARACTER
%token <c_double> C_DOUBLE
%token <c_float> C_FLOAT
%token <c_boolean> C_BOOLEAN
%token <c_string> C_STRING

/** ===== Arithmetic Types ===== */
%token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB

/** ===== Atomics ===== */
%token <token> COLON
%token <token> SEMICOLON
%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_PARENTHESIS
%token <token> APOSTROPHE
%token <token> QUOTE

%token <token> ASSIGN
%token <token> ADD_ASSIGN
%token <token> SUB_ASSIGN
%token <token> MUL_ASSIGN

%token <token> UNKNOWN

/** ============== TERMINALS ENDS. ============== */

/** ============== NON-TERMINALS. ============== */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

%type <assignmentOperation> assignment_operation

%type <variableDeclaration> variable_declaration
%type <varType> variable_type
/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%start program
%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: 
	assignment_operation											{ $$ = AssignmentProgramSemanticAction(currentCompilerState(), $1); }
	| variable_declaration											{ $$ = VariableProgramSemanticAction(currentCompilerState(), $1); }
	| expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

assignment_operation: 
	variable_declaration ASSIGN expression SEMICOLON				{ $$ = AssignmentDeclarationSemanticAction($1, $3, ASSIGN_TYPE); }
	| NAME ASSIGN expression SEMICOLON								{ $$ = AssignmentOperatorSemanticAction($1, $3, ASSIGN_TYPE); }
	| NAME ADD_ASSIGN expression SEMICOLON							{ $$ = AssignmentOperatorSemanticAction($1, $3, ADD_ASSIGN_TYPE); }
	| NAME SUB_ASSIGN expression SEMICOLON							{ $$ = AssignmentOperatorSemanticAction($1, $3, SUB_ASSIGN_TYPE); }
	| NAME MUL_ASSIGN expression SEMICOLON							{ $$ = AssignmentOperatorSemanticAction($1, $3, MUL_ASSIGN_TYPE); }
	;

variable_declaration:
	NAME COLON variable_type										{ $$ = VariableDeclarationSemanticAction($1, $3, NULL); }
	;

variable_type:
	TYPE 															{ $$ = $1; }
	;


expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: C_INTEGER													{ $$ = ConstantSemanticAction(&$1, sizeof(int),		C_INT_TYPE); }
		| C_CHARACTER												{ $$ = ConstantSemanticAction(&$1, sizeof(char),	C_CHAR_TYPE); }
		| C_STRING													{ $$ = ConstantSemanticAction(&$1, sizeof(char)*strlen($1) + 1,C_STRING_TYPE); }
		| C_DOUBLE													{ $$ = ConstantSemanticAction(&$1, sizeof(double),	C_DOUBLE_TYPE); }	
		| C_FLOAT													{ $$ = ConstantSemanticAction(&$1, sizeof(float),	C_FLOAT_TYPE); }
		| C_BOOLEAN													{ $$ = ConstantSemanticAction(&$1, sizeof(boolean),C_BOOLEAN_TYPE); }	
	;

%%
