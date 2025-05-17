#include "FlexActions.h"

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
void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return token;
}

Token ConstantLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	
	switch (token) {
		case C_INTEGER:
			lexicalAnalyzerContext->semanticValue->c_integer = atoi(lexicalAnalyzerContext->lexeme);
			break;
		case C_CHARACTER:
			lexicalAnalyzerContext->semanticValue->c_character = lexicalAnalyzerContext->lexeme[0];
			break;
		case C_DOUBLE:
			lexicalAnalyzerContext->semanticValue->c_double = atof(lexicalAnalyzerContext->lexeme);
			break;
		case C_FLOAT:
			lexicalAnalyzerContext->semanticValue->c_float = atof(lexicalAnalyzerContext->lexeme);
			break;
		case C_BOOLEAN:
			lexicalAnalyzerContext->semanticValue->c_boolean = lexicalAnalyzerContext->lexeme[0] == 't' ? true : false;
			break;
		case C_STRING:
			lexicalAnalyzerContext->semanticValue->c_string = strdup(lexicalAnalyzerContext->lexeme);
			break;
	}

	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return token;
}

Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return token;
}

Token ApostropheLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = APOSTROPHE;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return APOSTROPHE;
}

Token QuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = QUOTE;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return QUOTE;
}

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return UNKNOWN;
}


Token NameLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->name = strdup(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return NAME;	
}

Token SemicolonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = SEMICOLON;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);	
	return SEMICOLON;
}

Token ColonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = COLON;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return COLON;
}

Token TypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, VariableType varType) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->varType = varType;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return TYPE;
}

