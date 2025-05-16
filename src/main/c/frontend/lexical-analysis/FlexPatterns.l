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

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

digit			      [0-9]
char                  [a-zA-Z_]
name            {char}({char}|{digit})*

%%
"-"						            { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

":"                                 { return ColonLexemeAction(createLexicalAnalyzerContext()); }
";"                                 { return SemicolonLexemeAction(createLexicalAnalyzerContext()); }

"int"                               { return TypeLexemeAction(createLexicalAnalyzerContext(), V_INT); }
"char"                              { return TypeLexemeAction(createLexicalAnalyzerContext(), V_CHAR); } 
"float"                             { return TypeLexemeAction(createLexicalAnalyzerContext(), V_FLOAT); } 
"double"                            { return TypeLexemeAction(createLexicalAnalyzerContext(), V_DOUBLE); } 
"string"                            { return TypeLexemeAction(createLexicalAnalyzerContext(), V_STRING); } 
"long"                              { return TypeLexemeAction(createLexicalAnalyzerContext(), V_LONG); } 
"short"                             { return TypeLexemeAction(createLexicalAnalyzerContext(), V_SHORT); } 

[[:alpha:]]+                        { return NameLexemeAction(createLexicalAnalyzerContext()); }
[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }
%%

#include "FlexExport.h"
