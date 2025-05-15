%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue


%union {
	/** Terminals. */
	// Variables
	char * varname;

	// Variable types
	int intType;
	char charType;
	float floatType;
	double doubleType;
	char * stringType;
	bool boolType;
	long longType;
	short shortType;
	void * ptr;
	
	Token token;

	/** Non-terminals. */
	VariableDeclarationNode *variable_declaration;
	ExpresionNode *comparison_expression;
	ExpresionNode *arithmetic_expression;
	ConstantNode *constant;
	Program *program;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html

	%destructor { releaseConstant($$); } <constant>
	%destructor { releaseExpression($$); } <expression>
	%destructor { releaseFactor($$); } <factor>
 */

/** Terminals. */
%token <varname> VARIABLE_NAME
/** ===== Variable Types ===== */
%token <charType> CHAR
%token <intType> INTEGER
%token <doubleType> DOUBLE
%token <floatType> FLOAT
%token <stringType> STRING
%token <boolType> BOOLEAN
%token <longType> LONG
%token <shortType> SHORT
%token <ptr> NULL_LITERAL

/** ===== Atomics ===== */
%token <token> SEMICOLON
%token <token> COLON

/** ===== Assignartion ===== */
%token <token> ASIGNATION

/** ===== Comparison ===== */
%token <token> EQUAL
%token <token> GREATER
%token <token> GREATER_EQUAL
%token <token> LESS
%token <token> LESS_EQUAL
%token <token> NOT_EQUAL

/** ===== Arithmetic ===== */
%token <token> ADDITION
%token <token> DIVISION
%token <token> MULTIPLICATION
%token <token> SUBTRACTION


%token <token> UNKNOWN

/** Non-terminals. */
%type <variable_declaration> variable_declaration
%type <comparison_expression> comparison_expression
%type <arithmetic_expression> arithmetic_expression
%type <constant> constant
%type <program> program
/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
 
%left EQUAL NOT_EQUAL
%left LESS LESS_EQUAL GREATER GREATER_EQUAL

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
/**
program: expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	        4           +         3
	| factor														{ $$ = FactorExpressionSemanticAction($1); } // FACTOR Y NO LA COMIDA
	;
factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;

 */
program: 
	variable_declaration																{$$ = ProgramSemanticAction(currentCompilerState(),$1);}
	;

// var 
// var varname: type;
// var varname: type = expression;
// var varname: type = 3;
variable_declaration: 
	VARIABLE_NAME[varname] COLON TYPE[vartype] SEMICOLON			                    			{ $$ = VariableDeclarationSemanticAction(currentCompilerState(), $varname, $vartype); }
    |VARIABLE_NAME[varname] COLON TYPE[vartype] ASIGNATION comparison_expression[expr] SEMICOLON 	{ $$ = VariableDeclarationSemanticAction(currentCompilerState(), $varname, $vartype, $expr); }
	|VARIABLE_NAME[varname] COLON TYPE[vartype] ASIGNATION arithmetic_expression[expr] SEMICOLON 	{ $$ = VariableDeclarationSemanticAction(currentCompilerState(), $varname, $vartype, $expr); }
	|VARIABLE_NAME[varname] COLON TYPE[vartype] ASIGNATION constant[expr] SEMICOLON 				{ $$ = VariableDeclarationSemanticAction(currentCompilerState(), $varname, $vartype, $expr); }
	;
	

comparison_expression:
//True or False  
	comparison_expression[left] EQUAL comparison_expression[right]	 					{ $$ = ComparisonExpressionSemanticAction($left, $right, EQUAL); }
	|comparison_expression[left] GREATER comparison_expression[right]					{ $$ = ComparisonExpressionSemanticAction($left, $right, GREATER); }
	|comparison_expression[left] GREATER_EQUAL comparison_expression[right]				{ $$ = ComparisonExpressionSemanticAction($left, $right, GREATER_EQUAL); }
	|comparison_expression[left] LESS comparison_expression[right]						{ $$ = ComparisonExpressionSemanticAction($left, $right, LESS); } 
	|comparison_expression[left] LESS_EQUAL comparison_expression[right]				{ $$ = ComparisonExpressionSemanticAction($left, $right, LESS_EQUAL); } 
	|comparison_expression[left] NOT_EQUAL comparison_expression[right]					{ $$ = ComparisonExpressionSemanticAction($left, $right, NOT_EQUAL); } 
	|constant																			{ $$ = ConstantExpresionSemanticAction($1); }	
	;

arithmetic_expression:
// exp op exp = something
	arithmetic_expression[left] ADDITION arithmetic_expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); } 
	|arithmetic_expression[left] DIVISION arithmetic_expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
 	|arithmetic_expression[left] MULTIPLICATION arithmetic_expression[right]			{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	|arithmetic_expression[left] SUBTRACTION arithmetic_expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	|constant																			{ $$ = ConstantExpresionSemanticAction($1); }	
	;



constant:
	//char: 'x'
	CHAR[char] 																			{ $$ = CharConstantSemanticAction($char); }
	//Integer: 3
	|INTEGER[integer] 																	{ $$ = IntegerConstantSemanticAction($integer); }
	//Double: 3.0
	|DOUBLE[double] 																	{ $$ = FloatConstantSemanticAction($float); }
	//Float: 3.0f
	|FLOAT[float] 																		{ $$ = FloatConstantSemanticAction($float); }
	//String: "Tres"
	|STRING[string] 										 							{ $$ = StringConstantSemanticAction($string); }
	//Boolean: true or false
	|BOOLEAN[bool] 																		{ $$ = BooleanConstantSemanticAction($bool); }
	//Long: 3L
	|LONG[long] 																		{ $$ = LongConstantSemanticAction($long); }
	//Short: 3S
	|SHORT[short] 																		{ $$ = ShortConstantSemanticAction($short); }
	//Null: "NULL"
	|NULL[null] 																		{ $$ = NullConstantSemanticAction($null); }
	;
%%
