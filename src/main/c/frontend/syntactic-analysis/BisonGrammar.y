%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue


%union {
	/** Terminals. */
	// Variables
	int varType;
	char * identifier;

	// Variable types
	int intType;
	char charType;
	float floatType;
	double doubleType;
	char * stringType;

	boolean boolType;
	long longType;
	short shortType;
	void * ptr;
	
	Token token;

	/** Non-terminals. */
	VariableDeclarationNode *variable_declaration;
	// VariableTypeNode *variable_type;
	ExpressionNode *comparison_expression;
	ExpressionNode *arithmetic_expression;
	ExpressionNode *expression;
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
%token <identifier> IDENTIFIER
%token <varType> TYPE
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

/** ===== Assignment ===== */
%token <token> ASIGNATION
%token <token> ADD_ASIGNATION
%token <token> SUB_ASIGNATION
%token <token> MUL_ASIGNATION

/** ===== Comparison ===== */
 %token <token> EQUAL_SYMBOL 
%token <token> GREATER_SYMBOL
%token <token> GREATER_EQUAL_SYMBOL
%token <token> LESS_SYMBOL
%token <token> LESS_EQUAL_SYMBOL
%token <token> NOT_EQUAL_SYMBOL

/** ===== Arithmetic ===== */
%token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB

/** ===== Parenthesis ===== */
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <token> UNKNOWN

/** Non-terminals. */
%type <variable_declaration> variable_declaration
%type <expression> expression
%type <comparison_expression> comparison_expression
%type <arithmetic_expression> arithmetic_expression 
%type <constant> constant
%type <varType> variable_type
%type <program> program
/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
 
/* %left EQUAL NOT_EQUAL
%left LESS LESS_EQUAL GREATER GREATER_EQUAL
%left ADD SUB
%left MUL DIV
%left OPEN_PARENTHESIS CLOSE_PARENTHESIS */

%start program

%%

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
	TYPE 																	{ $$ = $1; }
	;

expression:
     constant              { $$ = ConstantExpressionSemanticAction($1); }
    //| IDENTIFIER            { $$ = IdentifierExpressionSemanticAction($1); }
    //| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS { $$ = ParenthesisExpressionSemanticAction($2); }
    ;

 comparison_expression:
    expression EQUAL_SYMBOL expression	 					{ $$ = ComparisonExpressionSemanticAction($1, $3, EQUAL); }
	| expression GREATER_SYMBOL expression					{ $$ = ComparisonExpressionSemanticAction($1, $3, GREATER); }
	| expression GREATER_EQUAL_SYMBOL expression			{ $$ = ComparisonExpressionSemanticAction($1, $3, GREATER_EQUAL); }
	| expression LESS_SYMBOL expression						{ $$ = ComparisonExpressionSemanticAction($1, $3, LESS); } 
	| expression LESS_EQUAL_SYMBOL expression				{ $$ = ComparisonExpressionSemanticAction($1, $3, LESS_EQUAL); } 
	| expression NOT_EQUAL_SYMBOL expression				{ $$ = ComparisonExpressionSemanticAction($1, $3, NOT_EQUAL); } 
	;

arithmetic_expression:
	expression ADD expression					{ $$ = ArithmeticExpressionSemanticAction($1, $3, ADD); } 
	| expression DIV expression					{ $$ = ArithmeticExpressionSemanticAction($1, $3, DIV); }
 	| expression MUL expression					{ $$ = ArithmeticExpressionSemanticAction($1, $3, MUL); }
	| expression SUB expression					{ $$ = ArithmeticExpressionSemanticAction($1, $3, SUB); } 
	;

constant:
	CHAR 										{ $$ = CharConstantSemanticAction($1); } 
	| INTEGER 									{ $$ = IntConstantSemanticAction($1); }
	| DOUBLE									{ $$ = DoubleConstantSemanticAction($1); }
	| FLOAT 									{ $$ = FloatConstantSemanticAction($1); }
	| STRING									{ $$ = StringConstantSemanticAction($1); }
	| BOOLEAN									{ $$ = BooleanConstantSemanticAction($1); }
	| LONG										{ $$ = LongConstantSemanticAction($1); }
	| SHORT 									{ $$ = ShortConstantSemanticAction($1); }
	;
	
%%

/* 1
'a'
12.0
12.0f
"String"
... */


/**
	expresion izq operador expresion der.


	varName semicolon type ;
	varName semicolon type = expresion ;
**/