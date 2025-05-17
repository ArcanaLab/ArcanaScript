%{

#include "FlexActions.h"
#include <stdlib.h>

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
%x CHAR_CONSTANT
%x STRING_CONSTANT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

digit			      [0-9]
char                  [a-zA-Z_]
name                  {char}({char}|{digit})*
single_line_comment     "#"[^!].*

%%
"#!"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^!]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"!#"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }

<INITIAL>{single_line_comment}               { IgnoredLexemeAction(createLexicalAnalyzerContext()); }  

"-"						            { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

"'"                                 { BEGIN(CHAR_CONSTANT); ApostropheLexemeAction(createLexicalAnalyzerContext()); }
<CHAR_CONSTANT>[^(\n|')]             { return ConstantLexemeAction(createLexicalAnalyzerContext(), C_CHARACTER); }
<CHAR_CONSTANT>"'"                  { ApostropheLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }

"\""                                { BEGIN(STRING_CONSTANT); QuoteLexemeAction(createLexicalAnalyzerContext()); }
<STRING_CONSTANT>[^(\n|\")]+        { return ConstantLexemeAction(createLexicalAnalyzerContext(), C_STRING); }
<STRING_CONSTANT>\"                  { QuoteLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }

<INITIAL>":"                        { return ColonLexemeAction(createLexicalAnalyzerContext()); }
";"                                 { return SemicolonLexemeAction(createLexicalAnalyzerContext()); }

"int"                               { return TypeLexemeAction(createLexicalAnalyzerContext(), V_INT); }
"char"                              { return TypeLexemeAction(createLexicalAnalyzerContext(), V_CHAR); } 
"float"                             { return TypeLexemeAction(createLexicalAnalyzerContext(), V_FLOAT); } 
"double"                            { return TypeLexemeAction(createLexicalAnalyzerContext(), V_DOUBLE); } 
"string"                            { return TypeLexemeAction(createLexicalAnalyzerContext(), V_STRING); } 
"long"                              { return TypeLexemeAction(createLexicalAnalyzerContext(), V_LONG); } 
"short"                             { return TypeLexemeAction(createLexicalAnalyzerContext(), V_SHORT); } 
"boolean"                           { return TypeLexemeAction(createLexicalAnalyzerContext(), V_BOOLEAN); } 

"while"                             { return LoopLexemeAction(createLexicalAnalyzerContext(), WHILE); }
"for"                               { return LoopLexemeAction(createLexicalAnalyzerContext(), FOR); }

"="                                 { return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), ASSIGN); }
"+="                                { return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), ADD_ASSIGN); }
"-="                                { return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), SUB_ASSIGN); }
"*="                                { return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), MUL_ASSIGN); }

"true"|"false"                      { return ConstantLexemeAction(createLexicalAnalyzerContext(), C_BOOLEAN); }
[0-9]+\.[0-9]+f						{ return ConstantLexemeAction(createLexicalAnalyzerContext(), C_FLOAT); }
[0-9]+\.[0-9]+						{ return ConstantLexemeAction(createLexicalAnalyzerContext(), C_DOUBLE); }
[0-9]+						        { return ConstantLexemeAction(createLexicalAnalyzerContext(), C_INTEGER); }


{name}                              { return NameLexemeAction(createLexicalAnalyzerContext()); }
[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }
%%

#include "FlexExport.h"
