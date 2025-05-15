%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
* Reusable patterns.
*
* @see https://westes.github.io/flex/manual/Matching.html
* @see https://westes.github.io/flex/manual/Patterns.html
*/

%%
#pragma region Patterns From Base TP

    /**

    "-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
    "*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
    "/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
    "+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }
    "("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
    ")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

    [[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

    [[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
    .									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

    */

#pragma endregion


#pragma region Atomics

":"          { return COLON;}
";"          { return SEMICOLON;}

#pragma endregion


#pragma region Comments

    /**
    * ============================================================================
    *                                 MULTI LINE
    * ============================================================================
    */
    "#*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
    <MULTILINE_COMMENT>"*#"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
    <MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
    <MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
    <MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

    /**
    * ============================================================================
    *                                 SINGLE LINE
    * ============================================================================
    */

    "#"[^\n]*                           { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

#pragma endregion

#pragma region VariableTypes

    "char"     { yylval.varType = CHAR_TYPE; return TYPE; }
    "int"      { yylval.varType = INT_TYPE; return TYPE; }
    "double"   { yylval.varType = DOUBLE_TYPE; return TYPE; }
    "float"    { yylval.varType = FLOAT_TYPE; return TYPE; }
    "boolean"  { yylval.varType = BOOLEAN_TYPE; return TYPE; }
    "long"     { yylval.varType = LONG_TYPE; return TYPE; }
    "short"    { yylval.varType = SHORT_TYPE; return TYPE; }

#pragma endregion

#pragma region AsignmentOperator
    "="									{ return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), ASIGNATION); }
    "+="								{ return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), ADD_ASIGNATION); }
    "-="								{ return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), SUB_ASIGNATION); }
    "*="								{ return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), MUL_ASIGNATION); }
#pragma endregion

#pragma region ComparisonOperator
    
    "=="								{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), EQUAL); }
    "!="								{ return CoparisonOperatorLexemeAction(createLexicalAnalyzerContext(), NOT_EQUAL); }
    "<"									{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS); }
    "<="								{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS_EQUAL); }
    ">"									{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER); }
    ">="								{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER_EQUAL); }

#pragma endregion


%%

#include "FlexExport.h"
