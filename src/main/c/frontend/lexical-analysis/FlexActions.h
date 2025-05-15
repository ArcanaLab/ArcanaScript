#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

#pragma region Base TP Lexeme Actions

   Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
   Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
   Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

#pragma endregion


#pragma region Comment Lexeme Actions

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#pragma endregion

VariableDeclarationNode * VariableSemanticAction(char * name, TypeEnum * type);

#pragma region Comparison Lexeme Actions

Token ComparisonOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

#pragma endregion

#pragma region Unknown Lexeme Actions

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#pragma endregion

#endif
