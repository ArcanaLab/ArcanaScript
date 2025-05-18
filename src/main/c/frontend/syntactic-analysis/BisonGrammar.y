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

	Instruction * instruction;
	Block * block;

	Lambda * lambda;
	VariableDeclarationList * varList;
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
%destructor { releaseName($$); } <name>
%destructor { releaseVariableDeclaration($$); } <variableDeclaration>
%destructor { releaseAssignmentOperation($$); } <assignmentOperation>
%destructor { releaseInstruction($$); } <instruction>
%destructor { releaseBlock($$); } <block>
%destructor { releaseLambda($$); } <lambda>

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

%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE

%token <token> COMMA

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

%type <variableDeclaration> variable_declaration
%type <varType> variable_type

%type <assignmentOperation> assignment_operation

%type <lambda> lambda
%type <varList> var_list

%type <instruction> instruction
%type <block> block
%type <block> scope
%type <program> program
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
/**
 * Multiple instructions. (Blocks)
 *
 * Parser will read all lines of the input file and try to parse them to an program.
 * So program MUST have at least a block.
 */
program: 
	block															{ $$ = BlockProgramSemanticAction(currentCompilerState(), $1); }
	;

block:
	instruction														{ $$ = CreateBlockSemanticAction($1); }
	| block instruction												{ $$ = AppendInstructionSemanticAction($1, $2); }
	;

instruction:
	assignment_operation SEMICOLON									{ $$ = InstructionSemanticAction($1, INSTRUCTION_ASSIGNMENT); }
	| variable_declaration SEMICOLON								{ $$ = InstructionSemanticAction($1, INSTRUCTION_VARIABLE_DECLARATION); }
	| expression SEMICOLON											{ $$ = InstructionSemanticAction($1, INSTRUCTION_EXPRESSION); }
	| scope															{ $$ = InstructionSemanticAction($1, INSTRUCTION_BLOCK); }
	;

scope:
	OPEN_BRACE block CLOSE_BRACE									{ $$ = $2; }
	| OPEN_BRACE CLOSE_BRACE										{ $$ = CreateBlockSemanticAction(NULL); }

assignment_operation: 
	NAME ASSIGN expression											{ $$ = AssignmentOperatorSemanticAction($1, $3, ASSIGN_TYPE); }
	| NAME ADD_ASSIGN expression									{ $$ = AssignmentOperatorSemanticAction($1, $3, ADD_ASSIGN_TYPE); }
	| NAME SUB_ASSIGN expression									{ $$ = AssignmentOperatorSemanticAction($1, $3, SUB_ASSIGN_TYPE); }
	| NAME MUL_ASSIGN expression									{ $$ = AssignmentOperatorSemanticAction($1, $3, MUL_ASSIGN_TYPE); }
	// TODO: Debatir mejor opción:
	// - Si es en assignment_operation o variable_declaration que hay que poner esto:
	/* | variable_declaration ASSIGN expression SEMICOLON				{ $$ = AssignmentDeclarationSemanticAction($1, $3); } */
	;

variable_declaration:
	NAME COLON variable_type										{ $$ = VariableDeclarationSemanticAction($1, $3, NULL); }
	| NAME COLON variable_type ASSIGN expression					{ $$ = VariableDeclarationSemanticAction($1, $3, $5); }
	;

variable_type:
	TYPE 															{ $$ = $1; }
	;


expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	| lambda														{ $$ = LambdaExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

lambda: OPEN_PARENTHESIS CLOSE_PARENTHESIS instruction				{ $$ = LambdaSemanticAction(NULL, $3); }
	| OPEN_PARENTHESIS var_list CLOSE_PARENTHESIS instruction		{ $$ = LambdaSemanticAction($2, $4); }
	;

var_list: variable_declaration										{ $$ = CreateVariableDeclarationListSemanticAction($1); }
	| var_list COMMA variable_declaration							{ $$ = AppendVariableDeclarationSemanticAction($1, $3); }
	;

constant: C_INTEGER													{ $$ = ConstantSemanticAction(&$1, C_INT_TYPE); }
		| C_CHARACTER												{ $$ = ConstantSemanticAction(&$1, C_CHAR_TYPE); }
		| C_STRING													{ $$ = ConstantSemanticAction(&$1, C_STRING_TYPE); }
		| C_DOUBLE													{ $$ = ConstantSemanticAction(&$1, C_DOUBLE_TYPE); }	
		| C_FLOAT													{ $$ = ConstantSemanticAction(&$1, C_FLOAT_TYPE); }
		| C_BOOLEAN													{ $$ = ConstantSemanticAction(&$1, C_BOOLEAN_TYPE); }	
	;

%%
