#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "utils/ContextStack.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>
#include <string.h>

/** HAS ABORTED DUE TO CRITICAL ERROR */
int hasAborted(void);

// ================== [ Initialization ] ==================
#pragma region Initialization
/**
 * Initializes the internal state of the module.
 */
void initializeBisonActionsModule();

/**
 * Shuts down the internal state of the module.
 */
void shutdownBisonActionsModule();
#pragma endregion
// ========================================================

// ================== [ Constants ] =======================
#pragma region Constants
/**
 * Creates a constant with the specified value and type.
 * @param value The value of the constant.
 * @param constantType The type of the constant.
 * @return A pointer to the created constant.
 */
Constant * ConstantSemanticAction(const void * value, ConstantType constantType);
#pragma endregion
// ========================================================

// ================== [ Expressions ] =====================
#pragma region Expressions
/**
 * Creates an arithmetic expression with the specified left and right expressions, and type.
 * @param leftExpression The left expression.
 * @param rightExpression The right expression.
 * @param type The type of the expression.
 * @return A pointer to the created arithmetic expression.
 */
Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);

/**
 * Creates an expression from a factor.
 * @param factor The factor to convert into an expression.
 * @return A pointer to the created expression.
 */
Expression * FactorExpressionSemanticAction(Factor * factor);

/**
 * Creates a comparison expression with the specified left and right factors, and type.
 * @param leftFactor The left factor.
 * @param rightFactor The right factor.
 * @param type The type of the expression.
 * @return A pointer to the created comparison expression.
 */
Expression * ComparatorExpressionSemanticAction(Factor * leftFactor, Factor * rightFactor, ExpressionType type);

/**
 * Creates a variable expression with the specified name.
 * @param variable The name of the variable.
 * @return A pointer to the created variable expression.
 */
Expression * VariableExpressionSemanticAction(char * variable);

/**
 * Creates a unary expression with the specified name and type.
 * @param name The name of the unary expression.
 * @param type The type of the expression.
 * @return A pointer to the created unary expression.
 */
Expression * UnaryExpressionSemanticAction(char * name,  ExpressionType type);

/**
 * Creates an expression from a lambda.
 * @param lambda The lambda to convert into an expression.
 * @return A pointer to the created expression.
 */
Expression * LambdaExpressionSemanticAction(Lambda * lambda);

/**
 * Creates an expression from a function call.
 * @param functionCall The function call to convert into an expression.
 * @return A pointer to the created expression.
 */
Expression * FunctionCallExpressionSemanticAction(FunctionCall * functionCall);
#pragma endregion
// ========================================================

// ================== [ Factors ] =========================
#pragma region Factors
/**
 * Creates a factor from a constant.
 * @param constant The constant to convert into a factor.
 * @return A pointer to the created factor.
 */
Factor * ConstantFactorSemanticAction(Constant * constant);

/**
 * Creates a factor from an expression.
 * @param expression The expression to convert into a factor.
 * @return A pointer to the created factor.
 */
Factor * ExpressionFactorSemanticAction(Expression * expression);
#pragma endregion
// ========================================================

// ================== [ Conditionals ] ====================
#pragma region Conditionals
/**
 * Creates a conditional with the specified expression, type, and block.
 * @param expression The expression of the condition.
 * @param conditionalType The type of the condition.
 * @param block The block of the condition.
 * @return A pointer to the created conditional.
 */
Conditional * ConditionalSemanticAction(Expression * expression, ConditionalType conditionalType,Block * block);
#pragma endregion
// ========================================================

// ================== [ Loops ] ===========================
#pragma region Loops
/**
 * Creates a loop with the specified expression, type, block, item name, and collection name.
 * @param expression The expression of the loop.
 * @param type The type of the loop.
 * @param block The block of the loop.
 * @param itemName The name of the item.
 * @param collectionName The name of the collection.
 * @return A pointer to the created loop.
 */
Loop * LoopSemanticAction(Expression * expression, LoopType type, Block * block, char * itemName, char * collectionName);
#pragma endregion
// ========================================================

// ================== [ Declarations ] ====================
#pragma region Declarations
/**
 * Performs the semantic action for a variable declaration in the analyzer.
 * @param name The name of the variable being declared.
 * @param type The type of the variable.
 * @param expression The initial value or expression assigned to the variable, or nullptr if not initialized.
 * @param object The object context in which the variable is declared, or nullptr if not applicable.
 * @param privacyModifierList The list of privacy modifiers applied to the variable, or nullptr if none.
 * @return A pointer to the VariableDeclaration object representing the declaration.
 */
VariableDeclaration * VariableDeclarationSemanticAction(char * name, VariableType type, Expression * expression, Object * object, PrivacyList * privacyModifierList);

/**
 * Creates a variable declaration list with the specified list and declaration.
 * @param variableDeclarationList The existing list.
 * @param variableDeclaration The variable declaration to add.
 * @return A pointer to the created list.
 */
VariableDeclarationList * VariableDeclarationListSemanticAction(VariableDeclarationList * variableDeclarationList, VariableDeclaration * variableDeclaration);
#pragma endregion
// ========================================================

// ================== [ Assignments ] =====================
#pragma region Assignments
/**
 * Creates an assignment operation from a variable declaration and an expression.
 * @param variableDeclaration The variable declaration.
 * @param expression The expression to assign.
 * @return A pointer to the created assignment operation.
 */
AssignmentOperation * AssignmentDeclarationSemanticAction(VariableDeclaration * variableDeclaration, Expression * expression);

/**
 * Creates an assignment operation with the specified name, expression, and assignment operator type.
 * @param name The name of the variable.
 * @param expression The expression to assign.
 * @param assignmentOperatorType The type of assignment operator.
 * @return A pointer to the created assignment operation.
 */
AssignmentOperation * AssignmentOperatorSemanticAction(char * name, Expression * expression, AssignmentOperatorType assignmentOperatorType);
#pragma endregion
// ========================================================

// ================== [ Instructions ] ====================
#pragma region Instructions
/**
 * Creates an instruction with the specified value and type.
 * @param value The value of the instruction.
 * @param instructionType The type of the instruction.
 * @return A pointer to the created instruction.
 */
Instruction * InstructionSemanticAction(void * value, InstructionType instructionType);
#pragma endregion
// ========================================================
// ================== [ Imports ] ==========================
#pragma region Imports
/**
 * Creates an import statement with the specified module name.
 * @param path The path to the module to import with the name.
 * @return A pointer to the created import statement.
 */
Import * ImportSemanticAction(char * path);

/**
 * Creates an import list with the specified list and import statement.
 * @param importList The existing list of imports.
 * @param importStatement The import statement to add.
 * @return A pointer to the created import list.
 */
ImportList * ImportListSemanticAction(ImportList * importList, Import * importStatement);
#pragma endregion
// ================== [ Blocks ] ==========================
#pragma region Blocks
/**
 * Creates a block with the specified block and instruction.
 * @param block The existing block.
 * @param instruction The instruction to add.
 * @return A pointer to the created block.
 */
Block * BlockSemanticAction(Block * block, Instruction * instruction);
#pragma endregion
// ========================================================

// ================== [ Classes ] =========================
#pragma region Classes
/**
 * Creates a class with the specified object, inheritance, implementation list, and block.
 * @param object The class object.
 * @param inherits The object from which it inherits.
 * @param implementationList The list of implementations.
 * @param block The class block.
 * @return A pointer to the created class.
 */
Class * ClassSemanticAction(Object * object, Object * inherits, ImplementationList * implementationList, Block * block);
#pragma endregion
// ========================================================

// ================== [ Lambdas ] =========================
#pragma region Lambdas
/**
 * Creates a lambda with the specified variable declaration list and instruction.
 * @param variableDeclarationList The list of variable declarations.
 * @param instruction The lambda instruction.
 * @return A pointer to the created lambda.
 */
Lambda * LambdaSemanticAction(VariableDeclarationList * variableDeclarationList, Block * block);
#pragma endregion
// ========================================================

// ================== [ Privacy ] =========================
#pragma region Privacy
/**
 * Creates a privacy list with the specified list and modifier.
 * @param privacyList The existing list.
 * @param modifier The privacy modifier to add.
 * @return A pointer to the created list.
 */
PrivacyList * PrivacyListSemanticAction(PrivacyList * privacyList, PrivacyModifier * modifier);

/**
 * Creates a privacy modifier with the specified type.
 * @param privacyType The type of privacy.
 * @return A pointer to the created privacy modifier.
 */
PrivacyModifier * PrivacyModifierSemanticAction(PrivacyType privacyType);
#pragma endregion
// ========================================================

// ================== [ Functions ] =======================
#pragma region Functions
/**
 * Creates a function call with the specified name and expression list.
 * @param name The name of the function.
 * @param expressionList The list of expressions.
 * @return A pointer to the created function call.
 */
FunctionCall * FunctionCallSemanticAction(char * name, ExpressionList * expressionList);
#pragma endregion
// ========================================================

// ================== [ Expression Lists ] ================
#pragma region ExpressionLists
/**
 * Creates an expression list with the specified list and expression.
 * @param expressionList The existing list.
 * @param expression The expression to add.
 * @return A pointer to the created list.
 */
ExpressionList * ExpressionListSemanticAction(ExpressionList * expressionList, Expression * expression);
#pragma endregion
// ========================================================

// ================== [ Programs ] ========================
#pragma region Programs
/**
 * Creates a program from a block.
 * @param compilerState The compiler state.
 * @param block The program block.
 * @return A pointer to the created program.
 */
Program * BlockProgramSemanticAction(CompilerState * compilerState,ImportList * importList, Block * block);

/**
 * Creates a program from a loop.
 * @param compilerState The compiler state.
 * @param loop The program loop.
 * @return A pointer to the created program.
 */
Program * LoopProgramSemanticAction(CompilerState * compilerState, Loop * loop);
#pragma endregion
// ========================================================

// ================== [ Objects ] =========================
#pragma region Objects
/**
 * Creates an object with the specified name and generic list.
 * @param name The name of the object.
 * @param genericList The list of generics.
 * @return A pointer to the created object.
 */
Object * ObjectSemanticAction(char * name, GenericList * genericList);
#pragma endregion
// ========================================================

// ================== [ Generics ] ========================
#pragma region Generics
/**
 * Creates a generic with the specified object and constraint object.
 * @param object The generic object.
 * @param isObject The constraint object.
 * @return A pointer to the created generic.
 */
Generic * GenericSemanticAction(Object * object, Object * isObject);

/**
 * Creates a generic list with the specified list and generic.
 * @param genericList The existing list.
 * @param generic The generic to add.
 * @return A pointer to the created list.
 */
GenericList * GenericListSemanticAction(GenericList * genericList, Generic * generic);
#pragma endregion
// ========================================================

// ================== [ Interfaces ] ======================
#pragma region Interfaces
/**
 * Creates an interface with the specified object, implementation list, and block.
 * @param object The interface object.
 * @param implementationList The list of implementations.
 * @param block The interface block.
 * @return A pointer to the created interface.
 */
Interface * InterfaceSemanticAction(Object * object, ImplementationList * implementationList, Block * block);
#pragma endregion
// ========================================================

// ================== [ Implementations ] =================
#pragma region Implementations
/**
 * Creates an implementation list with the specified list and object.
 * @param implementationList The existing list.
 * @param object The object to add.
 * @return A pointer to the created list.
 */
ImplementationList * ImplementationListSemanticAction(ImplementationList * implementationList, Object * object);
#pragma endregion
// ========================================================

// ================== [ Lists ] ===========================
#pragma region Lists
/**
 * Creates a list with the specified data.
 * @param data The data of the list.
 * @return A pointer to the created list.
 */
List * CreateListSemanticAction(void * data);

/**
 * Creates a list with the specified list and data.
 * @param list The existing list.
 * @param data The data to add.
 * @return A pointer to the created list.
 */
List * ListSemanticAction(List * list, void * data);
#pragma endregion
// ========================================================

#endif
