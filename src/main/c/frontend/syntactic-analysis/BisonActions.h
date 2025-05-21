#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>
#include <string.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Constant * ConstantSemanticAction(const void * value, ConstantType constantType);
Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Expression * ComparatorExpressionSemanticAction(Factor * leftFactor, Factor * rightFactor, ExpressionType type);
Expression * VariableExpressionSemanticAction(char * variable);
Expression * UnaryExpressionSemanticAction(char * name,  ExpressionType type);

Conditional * ConditionalSemanticAction(Expression * expression, ConditionalType conditionalType,Block * block);


Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression);
Loop * LoopSemanticAction(Expression * expression, LoopType type, Block * block, char * itemName, char * collectionName);



/**
 * Variable declarations.
 * 
 */
VariableDeclaration * VariableDeclarationSemanticAction(char * name, VariableType type, Expression * expression, Object * object, PrivacyList * privacyModifierList);

/**
 * Assignment operations.
 */
AssignmentOperation * AssignmentDeclarationSemanticAction(VariableDeclaration * variableDeclaration, Expression * expression);
AssignmentOperation * AssignmentOperatorSemanticAction(char * name, Expression * expression, AssignmentOperatorType assignmentOperatorType);

/**
 * Instructions.
 */
Instruction * InstructionSemanticAction(void * value, InstructionType instructionType);

/**
 * Blocks.
 */
Block * BlockSemanticAction(Block * block, Instruction * instruction);

/**
 * Class.
 */
Class * ClassSemanticAction(Object * object, Object * inherits, ImplementationList * implementationList, Block * block);

/**
 * Lambdas.
 */
Lambda * LambdaSemanticAction(VariableDeclarationList * variableDeclarationList, Instruction * instruction);
Expression * LambdaExpressionSemanticAction(Lambda * lambda);
VariableDeclarationList * VariableDeclarationListSemanticAction(VariableDeclarationList * variableDeclarationList, VariableDeclaration * variableDeclaration);

/**
 * Privacy.
 */
PrivacyList * PrivacyListSemanticAction(PrivacyList * privacyList, PrivacyModifier * modifier);
PrivacyModifier * PrivacyModifierSemanticAction(PrivacyType privacyType);

/**
 * Function calls.
 */
FunctionCall * FunctionCallSemanticAction(char * name, ExpressionList * expressionList);
Expression * FunctionCallExpressionSemanticAction(FunctionCall * functionCall);
ExpressionList * ExpressionListSemanticAction(ExpressionList * expressionList, Expression * expression);

/**
 * Programs.
 */
Program * BlockProgramSemanticAction(CompilerState * compilerState, Block * block);
Program * LoopProgramSemanticAction(CompilerState * compilerState, Loop * loop);

/**
 * Objects.
 */
Object * ObjectSemanticAction(char * name, GenericList * genericList);

/**
 * Generics.
 */
Generic * GenericSemanticAction(Object * object, Object * isObject);
GenericList * GenericListSemanticAction(GenericList * genericList, Generic * generic);

/**
 * Interfaces.
 */
Interface * InterfaceSemanticAction(Object * object, ImplementationList * implementationList, Block * block);

/**
 * Inheritance.
 */
ImplementationList * ImplementationListSemanticAction(ImplementationList * implementationList, Object * object);

/**
 * Lists.
 */
List * CreateListSemanticAction(void * data);
List * ListSemanticAction(List * list, void * data);

#endif
