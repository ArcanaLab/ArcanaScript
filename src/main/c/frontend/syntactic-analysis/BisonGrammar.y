%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	char * string;
	
	Token token;
	
	/** Non-terminals. */
	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;

	VariableDeclaration * variableDeclaration;
	VariableType varType;
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

/** ============== TERMINALS. ============== */
%token <name> NAME
%token <varType> TYPE

/** ===== Factor Types ===== */
%token <integer> INTEGER // 1 no es int (type)

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



%token <token> UNKNOWN

/** ============== TERMINALS ENDS. ============== */

/** ============== NON-TERMINALS. ============== */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

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

program:  variable_declaration											{ $$ = VariableProgramSemanticAction(currentCompilerState(), $1); }
		| expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

variable_declaration:
	NAME COLON variable_type SEMICOLON									{ $$ = VariableDeclarationSemanticAction($1, $3, NULL); }
	;

variable_type:
	TYPE 																{ $$ = $1; }
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

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;

%%
