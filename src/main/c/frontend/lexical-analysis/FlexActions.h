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

/**
 * Flex lexeme processing actions.
 */
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token ConstantLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token BraceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

void ApostropheLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void QuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

//Nuestro
Token SemicolonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token NameLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ColonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token TypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, VariableType token); 

Token ComparatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token AssignmentOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

Token ConditionalStructureLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

#endif
