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
	VariableTypeNode *variable_type;
	ExpresionNode *comparison_expression;
	ExpresionNode *arithmetic_expression;
	ExpresionNode *expression;
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
%token <varname> IDENTIFIER
%token INT_TYPE CHAR_TYPE DOUBLE_TYPE FLOAT_TYPE STRING_TYPE BOOLEAN_TYPE LONG_TYPE SHORT_TYPE
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
%type <expression> expression
%type <comparison_expression> comparison_expression
%type <arithmetic_expression> arithmetic_expression
%type <constant> constant
%type <variable_type> variable_type
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
// varname: type;
// varname: type = expression;
// varname: type = 3;
// varname: type = varnameB;

variable_declaration: 
	IDENTIFIER COLON variable_type SEMICOLON 									{ $$ = VariableDeclarationSemanticAction($1, $3, NULL); }
	| IDENTIFIER COLON variable_type ASIGNATION expression SEMICOLON 			{ $$ = VariableDeclarationSemanticAction($1, $3, $5); }
	;

variable_type:
	INT_TYPE 																	{ $$ = INT_TYPE; }
	| CHAR_TYPE 																{ $$ = CHAR_TYPE }
	| FLOAT_TYPE 																{ $$ = DOUBLE_TYPE; }
	| DOUBLE_TYPE 																{ $$ = FLOAT_TYPE; }
	| STRING_TYPE 																{ $$ = STRING_TYPE; }
	| BOOLEAN_TYPE 																{ $$ = BOOLEAN_TYPE; }
	| LONG_TYPE 																{ $$ = LONG_TYPE; }
	| SHORT_TYPE 																{ $$ = LONG_TYPE; }

expression:
	arithmetic_expression { $$ = $1; }
    | comparison_expression { $$ = $1; }
    | constant              { $$ = ConstantExpressionSemanticAction($1); }
    | IDENTIFIER            { /* Handle variable references */ }
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
	CHAR 																			{ $$ = CharConstantSemanticAction($1); }
	//Integer: 3
	|INTEGER 																	    { $$ = IntegerConstantSemanticAction($1); }
	//Double: 3.0
	|DOUBLE																	        { $$ = FloatConstantSemanticAction($1); }
	//Float: 3.0f
	|FLOAT 																			{ $$ = DoubleConstantSemanticAction($1); }
	//String: "Tres"
	|STRING										 									{ $$ = StringConstantSemanticAction($1); }
	//Boolean: true or false
	|BOOLEAN																		{ $$ = BooleanConstantSemanticAction($1); }
	//Long: 3L
	|LONG																			{ $$ = LongConstantSemanticAction($1); }
	//Short: 3S
	|SHORT 																			{ $$ = ShortConstantSemanticAction($1); }
	//Null: "NULL"
	|NULL_LITERAL																	{ $$ = NullConstantSemanticAction($1); }
	;
%%
