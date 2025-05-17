#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

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
Conditional * ConditionalSemanticAction(Expression * expression, ConditionalType conditionalType);


Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression);

VariableDeclaration * VariableDeclarationSemanticAction(char * name, VariableType type, Expression * expression);

Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression);
Program * VariableProgramSemanticAction(CompilerState * compilerState, VariableDeclaration * varDeclaration);

/**
 * Assignment operations.
 */
Program * AssignmentProgramSemanticAction(CompilerState * compilerState, AssignmentOperation * assignmentOperation);
Program * ConditionalProgramSemanticAction(CompilerState * compilerState, Conditional * conditional);

AssignmentOperation * AssignmentDeclarationSemanticAction(VariableDeclaration * variableDeclaration, Expression * expression);
AssignmentOperation * AssignmentOperatorSemanticAction(char * name, Expression * expression, AssignmentOperatorType assignmentOperatorType);

#endif
