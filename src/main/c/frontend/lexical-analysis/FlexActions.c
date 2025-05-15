#include "FlexActions.h"
#include <string.h>

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}


// Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
// 	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
// 	lexicalAnalyzerContext->semanticValue->token = token;
// 	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
// 	return token;
// }

// Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
// 	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
// 	lexicalAnalyzerContext->semanticValue-> = atoi(lexicalAnalyzerContext->lexeme);
// 	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
// 	return INTEGER;
// }

// Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
// 	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
// 	lexicalAnalyzerContext->semanticValue->token = token;
// 	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
// 	return token;
// }

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext) {
	char * escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
		functionName,
		escapedLexeme,
		lexicalAnalyzerContext->currentContext,
		lexicalAnalyzerContext->length,
		lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

// void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
// 	if (_logIgnoredLexemes) {
// 		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
// 	}
// 	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
// }

// void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
// 	if (_logIgnoredLexemes) {
// 		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
// 	}
// 	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
// }

// void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
// 	if (_logIgnoredLexemes) {
// 		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
// 	}
// 	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
// }



Token ComparisonOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return token;
}

Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->intType = atoi(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return INT_TYPE;
}

Token CharLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->charType = lexicalAnalyzerContext->lexeme[0];
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return CHAR_TYPE;
}
Token StringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->stringType = strdup(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return STRING_TYPE;
}
Token FloatLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->floatType = (float) atof(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return FLOAT_TYPE;
}
Token DoubleLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->doubleType = atof(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return DOUBLE_TYPE;
}
Token ShortLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->shortType = (short) atoi(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return SHORT_TYPE;
}
Token LongLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->longType = (long) atoi(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return LONG_TYPE;
}

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return UNKNOWN;
}

Token BooleanLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	if (strcmp(lexicalAnalyzerContext->lexeme, "true") == 0) {
		lexicalAnalyzerContext->semanticValue->boolType = true;
	} else if (strcmp(lexicalAnalyzerContext->lexeme, "false") == 0) {
		lexicalAnalyzerContext->semanticValue->boolType = false;
	} else {
		logError(_logger, "Invalid boolean value: %s", lexicalAnalyzerContext->lexeme);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return BOOLEAN_TYPE;
}

Token SemicolonLexemeAction() {
	yylval.token = SEMICOLON;
	return SEMICOLON;
}

Token ColonLexemeAction() {
	yylval.token = COLON;
	return COLON;
}
//Colon and Semicolon
Token IdentifierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->variable_declaration->identifier = strdup(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

}
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}
