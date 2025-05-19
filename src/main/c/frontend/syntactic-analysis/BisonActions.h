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

Conditional * ConditionalSemanticAction(Expression * expression, ConditionalType conditionalType,Block * block);


Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression);
Loop * LoopSemanticAction(Expression * expression, LoopType type, Block * block, char * itemName, char * collectionName);



/**
 * Variable declarations.
 * 
 */
VariableDeclaration * VariableDeclarationSemanticAction(char * name, VariableType type, Expression * expression);

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
Block * CreateBlockSemanticAction(Instruction * instruction);
Block * AppendInstructionSemanticAction(Block * block, Instruction * instruction);

/**
 * Lambdas.
 */
Lambda * LambdaSemanticAction(VariableDeclarationList * variableDeclarationList, Instruction * instruction);
Expression * LambdaExpressionSemanticAction(Lambda * lambda);
VariableDeclarationList * CreateVariableDeclarationListSemanticAction(VariableDeclaration * variableDeclaration);
VariableDeclarationList * AppendVariableDeclarationSemanticAction(VariableDeclarationList * variableDeclarationList, VariableDeclaration * variableDeclaration);
/**
 * Function calls.
 */
FunctionCall * FunctionCallSemanticAction(char * name, ExpressionList * expressionList);
ExpressionList * CreateExpressionListSemanticAction(Expression * expression);
ExpressionList * AppendExpressionSemanticAction(ExpressionList * expressionList, Expression * expression);
Expression * FunctionCallExpressionSemanticAction(FunctionCall * functionCall);

/**
 * Programs.
 */
Program * BlockProgramSemanticAction(CompilerState * compilerState, Block * block);
Program * LoopProgramSemanticAction(CompilerState * compilerState, Loop * loop);

#endif
