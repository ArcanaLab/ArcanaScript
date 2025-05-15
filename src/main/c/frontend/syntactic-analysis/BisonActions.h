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


Program * ProgramSemanticAction(CompilerState * compilerState, ExpressionNode * expression);
VariableDeclarationNode * VariableSemanticAction(char * name, TypeEnum * type, ExpressionNode * expression);
ExpressionNode * ComparisonExpressionSemanticAction(ExpressionNode * leftExpression, ExpressionNode * rightExpression, TypeExpression comparisonOperator);
ExpressionNode * ArithmeticExpressionSemanticAction(ExpressionNode * leftExpression, ExpressionNode * rightExpression, TypeExpression arithmeticOperator);

/** ===== Constants ===== */
ConstantNode * CharConstantSemanticAction(const char value);
ConstantNode * IntConstantSemanticAction(const int value);
ConstantNode * FloatConstantSemanticAction(const float value);
ConstantNode * DoubleConstantSemanticAction(const double value);
ConstantNode * StringConstantSemanticAction(const char * value);
ConstantNode * BooleanConstantSemanticAction(const boolean value);
ConstantNode * LongConstantSemanticAction(const long value);
ConstantNode * ShortConstantSemanticAction(const short value);


#pragma region "Unused Code"
/*
Constant * IntegerConstantSemanticAction(const int value);
Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression);
Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression);
*/
#pragma endregion 

#endif
