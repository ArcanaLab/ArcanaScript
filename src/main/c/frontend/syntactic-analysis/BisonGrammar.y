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
	Conditional * conditional;

	Expression * expression;
	Expression * comparator_expression;

	Factor * factor;
	Program * program;
	Loop * loop;

	PrivacyModifier * privacy;
	PrivacyList * privacyList;	
	VariableDeclaration * variableDeclaration;
	VariableType varType;

	AssignmentOperation * assignmentOperation;
	AssignmentOperatorType assignmentOperatorType;
	char * name;

	Instruction * instruction;
	Block * block;

	Lambda * lambda;
	VariableDeclarationList * varList;

	FunctionCall * functionCall;
	ExpressionList * expressionList;

	Object * object;
	Generic * generic;
	GenericList * genericList;
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
%destructor { releaseConditional($$); } <conditional>
%destructor { releaseAssignmentOperation($$); } <assignmentOperation>
%destructor { releaseInstruction($$); } <instruction>
%destructor { releaseBlock($$); } <block>
%destructor { releaseLoop($$); } <loop>
%destructor { releaseLambda($$); } <lambda>
%destructor { releaseFunctionCall($$); } <functionCall>
%destructor { releaseExpressionList($$); } <expressionList>
%destructor { releaseObject($$); } <object>
%destructor { releaseGeneric($$); } <generic>
%destructor { releaseGenericList($$); } <genericList>

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

/** ===== Privacy Types ===== */
%token <token> C_EXPOSED //Public
%token <token> C_HIDDEN //Private
%token <token> C_SHIELDED //Protected
%token <token> C_ETERNAL //Static
%token <token> C_IMMUTABLE //Const


/** ===== Arithmetic Types ===== */
%token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB

/** ===== Unary Operations ===== */
%token <token> INCREMENT
%token <token> DECREMENT

/** ===== Loops ===== */
%token <token> WHILE
%token <token> FOR

/** ===== Comparator ===== */
%token <token> GREATER
%token <token> GREATER_EQUAL
%token <token> LESS
%token <token> LESS_EQUAL
%token <token> EQUAL_EQUAL
%token <token> NOT_EQUAL


/** ===== Atomics ===== */
%token <token> COLON
%token <token> SEMICOLON
%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_PARENTHESIS

%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE

%token <token> COMMA

/** ===== Assignation ===== */
%token <token> ASSIGN
%token <token> ADD_ASSIGN
%token <token> SUB_ASSIGN
%token <token> MUL_ASSIGN

/** ===== CONDITIONAL ===== */
%token <token> IF
%token <token> ELSE

/** ===== CLASS ===== */
%token <token> IS
%token <token> USING

%token <token> UNKNOWN

/** ============== TERMINALS ENDS. ============== */

/** ============== NON-TERMINALS. ============== */
%type <constant> constant

%type <privacyList> privacy_list
%type <privacy> privacy_modifier

%type <expression> expression
%type <expression> comparator_expression
%type <conditional> if
%type <conditional> else

%type <factor> factor
%type <loop> loop

%type <variableDeclaration> variable_declaration
%type <varType> variable_type

%type <assignmentOperation> assignment_operation

%type <lambda> lambda
%type <varList> var_list

%type <functionCall> function_call
%type <expressionList> expression_list

%type <object> object
%type <generic> generic
%type <genericList> generic_list

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

%precedence ELSE
%precedence IF

%start program
%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
program: 
	block															{ $$ = BlockProgramSemanticAction(currentCompilerState(), $1); }
	;

block:
	instruction														{ $$ = BlockSemanticAction(NULL, $1); }
	| block instruction												{ $$ = BlockSemanticAction($1, $2); }
	;

instruction:
	assignment_operation SEMICOLON											{ $$ = InstructionSemanticAction($1, INSTRUCTION_ASSIGNMENT); }
	| variable_declaration SEMICOLON								{ $$ = InstructionSemanticAction($1, INSTRUCTION_VARIABLE_DECLARATION); }
	| expression SEMICOLON													{ $$ = InstructionSemanticAction($1, INSTRUCTION_EXPRESSION); }
	| scope																	{ $$ = InstructionSemanticAction($1, INSTRUCTION_BLOCK); }
	| loop 																	{ $$ = InstructionSemanticAction($1, INSTRUCTION_LOOP); }
	| if																	{ $$ = InstructionSemanticAction($1, INSTRUCTION_CONDITIONAL); }
	;

scope:
	OPEN_BRACE block CLOSE_BRACE									{ $$ = $2; }
	| OPEN_BRACE CLOSE_BRACE										{ $$ = BlockSemanticAction(NULL, NULL); }

loop:
	WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS scope 		{ $$ = LoopSemanticAction($3, WHILE_LOOP, $5, NULL, NULL); }	
	| FOR OPEN_PARENTHESIS NAME COLON NAME CLOSE_PARENTHESIS scope	{ $$ = LoopSemanticAction(NULL, FOR_LOOP, $7, $3, $5); }
	;

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
	NAME[name] COLON variable_type[type]																			{ $$ = VariableDeclarationSemanticAction($name, $type, NULL, NULL, NULL); }
	| NAME[name] COLON variable_type[type] ASSIGN expression[exp]													{ $$ = VariableDeclarationSemanticAction($name, $type, $exp, NULL, NULL); }
	| privacy_list[priv] NAME[name] COLON variable_type[type]														{ $$ = VariableDeclarationSemanticAction($name, $type, NULL, NULL, $priv); }
	| privacy_list[priv] NAME[name] COLON variable_type[type] ASSIGN expression[exp]								{ $$ = VariableDeclarationSemanticAction($name, $type, $exp, NULL, $priv); }
	
	| NAME[name] COLON object[obj]																				{ $$ = VariableDeclarationSemanticAction($name, OBJECT, NULL, $obj, NULL); }
	| NAME[name] COLON object[obj] ASSIGN expression[exp]														{ $$ = VariableDeclarationSemanticAction($name, OBJECT, $exp, $obj, NULL); }
	| privacy_list[priv] NAME[name] COLON object[obj]															{ $$ = VariableDeclarationSemanticAction($name, OBJECT, NULL, $obj, $priv); }
	| privacy_list[priv] NAME[name] COLON object[obj] ASSIGN expression[exp]									{ $$ = VariableDeclarationSemanticAction($name, OBJECT, $exp, $obj, $priv); }
	;

variable_type:
	TYPE 															{ $$ = $1; }
	;

if: IF OPEN_PARENTHESIS comparator_expression[exp] CLOSE_PARENTHESIS scope[block]   					{ $$ = ConditionalSemanticAction($exp,IF_TYPE,$block); }
    | IF OPEN_PARENTHESIS comparator_expression[exp] CLOSE_PARENTHESIS scope[block] else[con]			{ $$ = ConditionalSemanticAction($exp,IF_TYPE,$block); $$->nextConditional = $con; }	
	;
	
else:
	ELSE if																	{ $$ = $2; }
	| ELSE scope[block]														{ $$ = ConditionalSemanticAction(NULL,ELSE_TYPE,$block); }

comparator_expression: 	factor[left] GREATER factor[right]			{ $$ = ComparatorExpressionSemanticAction($left, $right, GREATER_TYPE); }
	| factor[left] GREATER_EQUAL factor[right]						{ $$ = ComparatorExpressionSemanticAction($left, $right, GREATER_EQUAL_TYPE); }
	| factor[left] LESS factor[right]								{ $$ = ComparatorExpressionSemanticAction($left, $right, LESS_TYPE); }
	| factor[left] LESS_EQUAL factor[right]							{ $$ = ComparatorExpressionSemanticAction($left, $right, LESS_EQUAL_TYPE); }
	| factor[left] EQUAL_EQUAL factor[right]						{ $$ = ComparatorExpressionSemanticAction($left, $right, EQUAL_EQUAL_TYPE); }
	| factor[left] NOT_EQUAL factor[right]							{ $$ = ComparatorExpressionSemanticAction($left, $right, NOT_EQUAL_TYPE); }

expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	| function_call													{ $$ = FunctionCallExpressionSemanticAction($1); }
	| lambda														{ $$ = LambdaExpressionSemanticAction($1); }
	| NAME															{ $$ = VariableExpressionSemanticAction($1); }	
	| NAME INCREMENT 												{ $$ = UnaryExpressionSemanticAction($1, INCREMENT_TYPE); }
	| NAME DECREMENT 												{ $$ = UnaryExpressionSemanticAction($1, DECREMENT_TYPE); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

lambda: OPEN_PARENTHESIS CLOSE_PARENTHESIS instruction				{ $$ = LambdaSemanticAction(NULL, $3); }
	| OPEN_PARENTHESIS var_list CLOSE_PARENTHESIS instruction		{ $$ = LambdaSemanticAction($2, $4); }
	;
	
var_list: variable_declaration										{ $$ = VariableDeclarationListSemanticAction(NULL, $1); }
	| var_list COMMA variable_declaration							{ $$ = VariableDeclarationListSemanticAction($1, $3); }
	;

function_call:
	NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS							{ $$ = FunctionCallSemanticAction($1, NULL); }
	| NAME OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS		{ $$ = FunctionCallSemanticAction($1, $3); }
	;
	
expression_list:
	expression														{ $$ = ExpressionListSemanticAction(NULL, $1); }
	| expression_list COMMA expression								{ $$ = ExpressionListSemanticAction($1, $3); }
	;

/** ===== Objects ===== */
object:
	NAME															{ $$ = ObjectSemanticAction($1, NULL); }
	| NAME LESS generic_list GREATER								{ $$ = ObjectSemanticAction($1, $3); }
	;

generic: 
	object															{ $$ = GenericSemanticAction($1, NULL); }
	| object IS object												{ $$ = GenericSemanticAction($1, $3); }

/** ===== Generics ===== */
generic_list:
	generic															{ $$ = GenericListSemanticAction(NULL, $1); }
	| generic_list COMMA generic									{ $$ = GenericListSemanticAction($1, $3); }
	;
	

constant: C_INTEGER													{ $$ = ConstantSemanticAction(&$1, C_INT_TYPE); }
		| C_CHARACTER												{ $$ = ConstantSemanticAction(&$1, C_CHAR_TYPE); }
		| C_STRING													{ $$ = ConstantSemanticAction(&$1, C_STRING_TYPE); }
		| C_DOUBLE													{ $$ = ConstantSemanticAction(&$1, C_DOUBLE_TYPE); }	
		| C_FLOAT													{ $$ = ConstantSemanticAction(&$1, C_FLOAT_TYPE); }
		| C_BOOLEAN													{ $$ = ConstantSemanticAction(&$1, C_BOOLEAN_TYPE); }	
	;

privacy_list: privacy_modifier[mod]									{ $$ = PrivacyListSemanticAction(NULL,$mod); }
	| privacy_list privacy_modifier[mod]							{ $$ = PrivacyListSemanticAction($1,$mod); }
	;
privacy_modifier:C_EXPOSED											{ $$ = PrivacyModifierSemanticAction(PUBLIC_A); }
	| C_HIDDEN														{ $$ = PrivacyModifierSemanticAction(PRIVATE_A); }
	| C_SHIELDED													{ $$ = PrivacyModifierSemanticAction(PROTECTED_A); }
	| C_ETERNAL														{ $$ = PrivacyModifierSemanticAction(STATIC_A); }
	| C_IMMUTABLE													{ $$ = PrivacyModifierSemanticAction(CONST_A);; }
	;


%%
