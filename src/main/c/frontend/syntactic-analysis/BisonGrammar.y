%{

#include "BisonActions.h"

#define HAS_CRITICAL_ABORT if(hasAborted() == 1) return 1;

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
	ImportList * importList;
	Import * importStatement;

	Constant * constant;
	Conditional * conditional;

	Expression * expression;
	Expression * comparatorExpression;

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

	Class * class;
	Interface * inter;

	FunctionCall * functionCall;
	ExpressionList * expressionList;

	Object * object;
	Generic * generic;
	GenericList * genericList;

	ImplementationList * implementationList;
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
%destructor { releaseClass($$); } <class>
%destructor { releaseVariableDeclarationList($$); } <varList>
%destructor { releaseImplementationList($$); } <implementationList>
%destructor { releasePrivacyList($$); } <privacyList>
%destructor { releasePrivacyModifier($$); } <privacy>
%destructor { releaseImportList($$); } <importList>
%destructor { releaseImport($$); } <importStatement>

/** ============== TERMINALS. ============== */
	// ------------------ [ Non-Tokens ] ------------------
		%token <name> NAME
		%token <varType> TYPE

	// ------------------ [ Expressions ] -----------------
		/** ===== Factor Types ===== */
		%token <c_integer> C_INTEGER
		%token <c_character> C_CHARACTER
		%token <c_double> C_DOUBLE
		%token <c_float> C_FLOAT
		%token <c_boolean> C_BOOLEAN
		%token <c_string> C_STRING

	// ------------------ [ Variables ] -------------------
		/** ===== Privacy Types ===== */
		%token <token> C_EXPOSED //Public
		%token <token> C_HIDDEN //Private
		%token <token> C_SHIELDED //Protected
		%token <token> C_ETERNAL //Static
		%token <token> C_IMMUTABLE //Const

	// ------------------ [ Comparasions ] ----------------
		/** ===== Comparator ===== */
		%token <token> GREATER
		%token <token> GREATER_EQUAL
		%token <token> LESS
		%token <token> LESS_EQUAL
		%token <token> EQUAL_EQUAL
		%token <token> NOT_EQUAL

	// ------------------ [ Operations ] ------------------
		/** ===== Arithmetic Types ===== */
		%token <token> ADD
		%token <token> DIV
		%token <token> MUL
		%token <token> SUB

		/** ===== Unary Operations ===== */
		%token <token> INCREMENT
		%token <token> DECREMENT

	// ------------------ [ Atomics ] ---------------------
		/** ===== Colon ===== */
		%token <token> COLON

		/** ===== Semicolon ===== */
		%token <token> SEMICOLON

		/** ===== Parentheses ===== */
		%token <token> CLOSE_PARENTHESIS
		%token <token> OPEN_PARENTHESIS

		/** ===== Braces ===== */
		%token <token> OPEN_BRACE
		%token <token> CLOSE_BRACE

		/** ===== Comma ===== */
		%token <token> COMMA

		/** ===== Assignation ===== */
		%token <token> ASSIGN
		%token <token> ADD_ASSIGN
		%token <token> SUB_ASSIGN
		%token <token> MUL_ASSIGN

	// ------------------ [ Control Structures ] ------------------
		/** ===== Loops ===== */
		%token <token> WHILE
		%token <token> FOR

		/** ===== CONDITIONAL ===== */
		%token <token> IF
		%token <token> ELSE

		/** ===== Control Flow ===== */
		%token <token> PASS

	// ------------------ [ Classes ] ---------------------
		/** ===== Imports ===== */
		%token <token> IMPORT
		%token <c_string> IMPORT_PATH

		/** ===== Class ===== */
		%token <token> CLASS

		/** ===== Interface ===== */
		%token <token> INTERFACE
		
		/** ===== Inheritance ===== */
		%token <token> IS
		%token <token> USING


	// ------------------ [ Functions ] -------------------
		/** ===== Return ===== */
		%token <token> RETURN
	// ------------------ [ Unknown ] ---------------------
		%token <token> UNKNOWN

/** ============== TERMINALS ENDS. ============== */

/** ============== NON-TERMINALS. ============== */
	// ------------------ [ Privacy ] ------------------
		%type <privacyList> privacy_list
		%type <privacy> privacy_modifier

	// ------------------ [ Constants ] ------------------
		%type <constant> constant

	// ------------------ [ Expressions ] ------------------
		%type <expression> expression
		%type <expression> comparator_expression
		%type <expressionList> expression_list

	// ------------------ [ Control ] ------------------
		/** ===== Conditionals ===== */
		%type <conditional> if
		%type <conditional> else

		/** ===== Loops ===== */
		%type <loop> loop

	// ------------------ [ Program Structure ] ------------------
		%type <importList> import_list
		%type <importStatement> import_statment
		%type <program> program
		%type <block> block
		%type <block> scope
		%type <instruction> instruction
		%type <factor> factor

	// ------------------ [ Variables ] ------------------
		%type <variableDeclaration> variable_declaration
		%type <varType> variable_type
	
	// ------------------ [ Operations ] ------------------
		/** ===== Assignament ===== */
			%type <assignmentOperation> assignment_operation

	// ------------------ [ Functions ] ------------------
		/** ===== Definition ===== */
			%type <lambda> lambda

		/** ===== Variables ===== */
			%type <varList> var_list
		/** ===== Function Call ===== */
			%type <functionCall> function_call
	// ------------------ [ OOP ] ------------------
		/** ===== Object ===== */
			%type <object> object

		/** ===== Classes ===== */
			%type <class> class
		/** ===== Generics ===== */
			%type <generic> generic
			%type <genericList> generic_list
		/** ===== Interfaces ===== */
			%type <inter> interface

		/** ===== Implementactions ===== */
			%type <implementationList> implementation
			%type <implementationList> implementation_list
		/** ===== Inheritance ===== */
			%type <object> inheritance_class
			%type <implementationList> inheritance_interface
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

	// ------------------ [ Program Structure ] ------------------
		program:
			import_list block																							{ $$ = BlockProgramSemanticAction(currentCompilerState(),$1,$2); }
			| block																										{ $$ = BlockProgramSemanticAction(currentCompilerState(),NULL ,$1); }
			;

		block:
			instruction																									{ $$ = BlockSemanticAction(NULL, $1); }
			| block instruction																							{ $$ = BlockSemanticAction($1, $2); }
			;

		import_list: import_statment																					{ $$ = ImportListSemanticAction(NULL,$1); }
			| import_list import_statment																				{ $$ = ImportListSemanticAction($1, $2); }	
			;

		import_statment:
			IMPORT C_STRING 																						  	{ $$ = ImportSemanticAction($2); }
			;
		instruction:
			assignment_operation SEMICOLON																				{ $$ = InstructionSemanticAction($1, INSTRUCTION_ASSIGNMENT); }
			| variable_declaration SEMICOLON																			{ $$ = InstructionSemanticAction($1, INSTRUCTION_VARIABLE_DECLARATION); }
			| expression SEMICOLON																						{ $$ = InstructionSemanticAction($1, INSTRUCTION_EXPRESSION); }
			| scope																										{ $$ = InstructionSemanticAction($1, INSTRUCTION_BLOCK); }
			| loop 																										{ $$ = InstructionSemanticAction($1, INSTRUCTION_LOOP); }
			| if																										{ $$ = InstructionSemanticAction($1, INSTRUCTION_CONDITIONAL); }
			| class																										{ $$ = InstructionSemanticAction($1, INSTRUCTION_CLASS); }
			| interface																									{ $$ = InstructionSemanticAction($1, INSTRUCTION_INTERFACE); }
			| RETURN instruction[ret]																					{ $$ = InstructionSemanticAction($ret, INSTRUCTION_RETURN); HAS_CRITICAL_ABORT }
			| RETURN SEMICOLON																							{ $$ = InstructionSemanticAction(NULL, INSTRUCTION_RETURN); HAS_CRITICAL_ABORT }
			| PASS SEMICOLON																							{ $$ = InstructionSemanticAction(NULL, INSTRUCTION_PASS); HAS_CRITICAL_ABORT }
			;

		scope:
			OPEN_BRACE block CLOSE_BRACE																				{ $$ = $2; }
			| OPEN_BRACE CLOSE_BRACE																					{ $$ = BlockSemanticAction(NULL, NULL); }

	// ------------------ [ Classes ] ----------------------------

		/** ===== DECLARATION ===== */

			class: 
				CLASS object[obj] { pushContext(CLASS_CONTEXT); } scope[scope_block]																	{ $$ = ClassSemanticAction($obj, NULL, NULL, $scope_block); popContext(); }
				| CLASS object[obj] inheritance_class[inherit] { pushContext(CLASS_CONTEXT); } scope[scope_block]										{ $$ = ClassSemanticAction($obj, $inherit, NULL, $scope_block); popContext(); }
				| CLASS object[obj] implementation[implement] { pushContext(CLASS_CONTEXT); } scope[scope_block]										{ $$ = ClassSemanticAction($obj, NULL, $implement, $scope_block); popContext(); }
				| CLASS object[obj] inheritance_class[inherit] implementation[implement] { pushContext(CLASS_CONTEXT); } scope[scope_block]				{ $$ = ClassSemanticAction($obj, $inherit, $implement, $scope_block); popContext(); }
				;

			interface:
				INTERFACE object[obj] { pushContext(INTERFACE_CONTEXT); } scope[scope_block]																						{$$ = InterfaceSemanticAction($obj, NULL, $scope_block); popContext(); }
				| INTERFACE object[obj] inheritance_interface[inherit] { pushContext(INTERFACE_CONTEXT); } scope[scope_block]														{$$ = InterfaceSemanticAction($obj, $inherit, $scope_block); popContext(); }

		/** ===== INHERITANCE ===== */

			inheritance_class:
				IS object																									{ $$ = $2; }
				;

			inheritance_interface:
				IS implementation_list																						{ $$ = $2; }
				;

			implementation:	
				USING implementation_list																					{ $$ = $2; }
				;

			implementation_list:
				object																										{ $$ = ImplementationListSemanticAction(NULL, $1); }
				| implementation_list COMMA object																			{ $$ = ImplementationListSemanticAction($1, $3); }
				;

	// ------------------ [ Control Structures ] -----------------
		/** ===== Loops ===== */
		loop:
			WHILE OPEN_PARENTHESIS expression[exp] CLOSE_PARENTHESIS { pushContext(LOOP_CONTEXT); } scope[scope_block] 											{ $$ = LoopSemanticAction($exp, WHILE_LOOP, $scope_block, NULL, NULL); popContext(); }	
			| FOR OPEN_PARENTHESIS NAME[item] COLON NAME[collection] CLOSE_PARENTHESIS { pushContext(LOOP_CONTEXT); } scope[scope_block]						{ $$ = LoopSemanticAction(NULL, FOR_LOOP, $scope_block, $item, $collection); popContext(); }
			;

		/** ===== Conditionals ===== */
		if: IF OPEN_PARENTHESIS comparator_expression[exp] CLOSE_PARENTHESIS scope[block]   							{ $$ = ConditionalSemanticAction($exp,IF_TYPE,$block); }
			| IF OPEN_PARENTHESIS comparator_expression[exp] CLOSE_PARENTHESIS scope[block] else[con]					{ $$ = ConditionalSemanticAction($exp,IF_TYPE,$block); $$->nextConditional = $con; }	
			;
			
		else:
			ELSE if																										{ $$ = $2; }
			| ELSE scope[block]																							{ $$ = ConditionalSemanticAction(NULL,ELSE_TYPE,$block); }
			;
	// ------------------ [ Variables ] --------------------------
		/** ===== Variable Declaration ===== */
		variable_declaration:
			NAME[name] COLON variable_type[type]																		{ $$ = VariableDeclarationSemanticAction($name, $type, NULL, NULL, NULL); }
			| NAME[name] COLON variable_type[type] ASSIGN expression[exp]												{ $$ = VariableDeclarationSemanticAction($name, $type, $exp, NULL, NULL); }
			| privacy_list[priv] NAME[name] COLON variable_type[type]													{ $$ = VariableDeclarationSemanticAction($name, $type, NULL, NULL, $priv); }
			| privacy_list[priv] NAME[name] COLON variable_type[type] ASSIGN expression[exp]							{ $$ = VariableDeclarationSemanticAction($name, $type, $exp, NULL, $priv); }
			
			| NAME[name] COLON object[obj]																				{ $$ = VariableDeclarationSemanticAction($name, OBJECT, NULL, $obj, NULL); }
			| NAME[name] COLON object[obj] ASSIGN expression[exp]														{ $$ = VariableDeclarationSemanticAction($name, OBJECT, $exp, $obj, NULL); }
			| privacy_list[priv] NAME[name] COLON object[obj]															{ $$ = VariableDeclarationSemanticAction($name, OBJECT, NULL, $obj, $priv); }
			| privacy_list[priv] NAME[name] COLON object[obj] ASSIGN expression[exp]									{ $$ = VariableDeclarationSemanticAction($name, OBJECT, $exp, $obj, $priv); }
			;
		/** ===== Variable Privacy ===== */
		privacy_list: privacy_modifier[mod]																				{ $$ = PrivacyListSemanticAction(NULL,$mod); }
			| privacy_list privacy_modifier[mod]																		{ $$ = PrivacyListSemanticAction($1,$mod); }
			;

		privacy_modifier:C_EXPOSED																						{ $$ = PrivacyModifierSemanticAction(PUBLIC_A); }
			| C_HIDDEN																									{ $$ = PrivacyModifierSemanticAction(PRIVATE_A); }
			| C_SHIELDED																								{ $$ = PrivacyModifierSemanticAction(PROTECTED_A); }
			| C_ETERNAL																									{ $$ = PrivacyModifierSemanticAction(STATIC_A); }
			| C_IMMUTABLE																								{ $$ = PrivacyModifierSemanticAction(CONST_A);; }
			;
			
		variable_type:
			TYPE 																										{ $$ = $1; }
			;

		/** ===== Constant ===== */
		constant: C_INTEGER																								{ $$ = ConstantSemanticAction(&$1, C_INT_TYPE); }
				| C_CHARACTER																							{ $$ = ConstantSemanticAction(&$1, C_CHAR_TYPE); }
				| C_STRING																								{ $$ = ConstantSemanticAction(&$1, C_STRING_TYPE); }
				| C_DOUBLE																								{ $$ = ConstantSemanticAction(&$1, C_DOUBLE_TYPE); }	
				| C_FLOAT																								{ $$ = ConstantSemanticAction(&$1, C_FLOAT_TYPE); }
				| C_BOOLEAN																								{ $$ = ConstantSemanticAction(&$1, C_BOOLEAN_TYPE); }	
			;

		/** ===== Objects ===== */
		object:
			NAME																										{ $$ = ObjectSemanticAction($1, NULL); }
			| NAME LESS generic_list GREATER																			{ $$ = ObjectSemanticAction($1, $3); }
			;
			
		/** ===== Generics ===== */
		generic: 
			object																										{ $$ = GenericSemanticAction($1, NULL); }
			| object IS object																							{ $$ = GenericSemanticAction($1, $3); }

		generic_list:
			generic																										{ $$ = GenericListSemanticAction(NULL, $1); }
			| generic_list COMMA generic																				{ $$ = GenericListSemanticAction($1, $3); }

		/** ===== Assignations ===== */
		assignment_operation: 
			NAME ASSIGN expression																						{ $$ = AssignmentOperatorSemanticAction($1, $3, ASSIGN_TYPE); }
			| NAME ADD_ASSIGN expression																				{ $$ = AssignmentOperatorSemanticAction($1, $3, ADD_ASSIGN_TYPE); }
			| NAME SUB_ASSIGN expression																				{ $$ = AssignmentOperatorSemanticAction($1, $3, SUB_ASSIGN_TYPE); }
			| NAME MUL_ASSIGN expression																				{ $$ = AssignmentOperatorSemanticAction($1, $3, MUL_ASSIGN_TYPE); }
			;
	// ------------------ [ Comparasions ] -----------------------
		comparator_expression: 	factor[left] GREATER factor[right]														{ $$ = ComparatorExpressionSemanticAction($left, $right, GREATER_TYPE); }
			| factor[left] GREATER_EQUAL factor[right]																	{ $$ = ComparatorExpressionSemanticAction($left, $right, GREATER_EQUAL_TYPE); }
			| factor[left] LESS factor[right]																			{ $$ = ComparatorExpressionSemanticAction($left, $right, LESS_TYPE); }
			| factor[left] LESS_EQUAL factor[right]																		{ $$ = ComparatorExpressionSemanticAction($left, $right, LESS_EQUAL_TYPE); }
			| factor[left] EQUAL_EQUAL factor[right]																	{ $$ = ComparatorExpressionSemanticAction($left, $right, EQUAL_EQUAL_TYPE); }
			| factor[left] NOT_EQUAL factor[right]																		{ $$ = ComparatorExpressionSemanticAction($left, $right, NOT_EQUAL_TYPE); }
			;

	// ------------------ [ Expressions ] ------------------------

		expression: expression[left] ADD expression[right]																{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
			| expression[left] DIV expression[right]																	{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
			| expression[left] MUL expression[right]																	{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
			| expression[left] SUB expression[right]																	{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
			| factor																									{ $$ = FactorExpressionSemanticAction($1); }
			| function_call																								{ $$ = FunctionCallExpressionSemanticAction($1); }
			| lambda																									{ $$ = LambdaExpressionSemanticAction($1); }
			;

		/** ===== Factor ===== */
		factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS															{ $$ = ExpressionFactorSemanticAction($2); }
			| NAME																										{ $$ = VariableExpressionSemanticAction($1); }	
			| NAME INCREMENT 																							{ $$ = UnaryExpressionSemanticAction($1, INCREMENT_TYPE); }
			| NAME DECREMENT 																							{ $$ = UnaryExpressionSemanticAction($1, DECREMENT_TYPE); }
			| constant																									{ $$ = ConstantFactorSemanticAction($1); }
			;

	// ------------------ [ Miscellaneous ] ----------------------

		/** ===== Lambda ===== */
		lambda: OPEN_PARENTHESIS CLOSE_PARENTHESIS { pushContext(LAMBDA_CONTEXT); } scope[scope_block]					{ $$ = LambdaSemanticAction(NULL, $scope_block); popContext(); }
			| OPEN_PARENTHESIS var_list[args] CLOSE_PARENTHESIS { pushContext(LAMBDA_CONTEXT); } scope[scope_block]											{ $$ = LambdaSemanticAction($args, $scope_block); popContext(); }
			;

		/** ===== Argument List (Lambda) ===== */
		var_list: variable_declaration																					{ $$ = VariableDeclarationListSemanticAction(NULL, $1); }
			| var_list COMMA variable_declaration																		{ $$ = VariableDeclarationListSemanticAction($1, $3); }
			;
			
		/** ===== Function Call ===== */
		function_call:
			NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS																		{ $$ = FunctionCallSemanticAction($1, NULL); }
			| NAME OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS													{ $$ = FunctionCallSemanticAction($1, $3); }
			;

		/** ===== Argument List (Function Call) ===== */
		expression_list:
			expression																									{ $$ = ExpressionListSemanticAction(NULL, $1); }
			| expression_list COMMA expression																			{ $$ = ExpressionListSemanticAction($1, $3); }
		;
	
%%
