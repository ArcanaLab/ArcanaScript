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
VariableDeclarationNode * VariableDeclarationSemanticAction(char * name, TypeEnum type, ExpressionNode * expression);
ExpressionNode * ComparisonExpressionSemanticAction(ExpressionNode * leftExpression, ExpressionNode * rightExpression, TypeExpression comparisonOperator);
ExpressionNode * ArithmeticExpressionSemanticAction(ExpressionNode * leftExpression, ExpressionNode * rightExpression, TypeExpression arithmeticOperator);


// ConstantNode * ConstantExpressionSemanticAction(const char value);
//VariableDeclarationNode * IdentifierExpressionSemanticAction(char * identifier, TypeEnum * type, ExpressionNode * expression);
/** ===== Constants ===== */
ConstantNode * ConstantExpressionSemanticAction(const char value);

ConstantNode * CharConstantSemanticAction(const char value);
ConstantNode * IntConstantSemanticAction(const int value);
ConstantNode * FloatConstantSemanticAction(const float value);
ConstantNode * DoubleConstantSemanticAction(const double value);
ConstantNode * StringConstantSemanticAction(const char * value);
ConstantNode * BooleanConstantSemanticAction(const boolean value);
ConstantNode * LongConstantSemanticAction(const long value);
ConstantNode * ShortConstantSemanticAction(const short value);


/** ===== Expressions ===== */
// ExpressionNode * IdentifierExpressionSemanticAction(const char value);
// ExpressionNode * ParenthesisExpressionSemanticAction(const char value);


#endif
