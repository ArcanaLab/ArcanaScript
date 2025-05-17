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

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

digit		    [0-9]
char            [a-zA-Z_]

/* "'"                                 { BEGIN(CHAR_CONSTANT); IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<CHAR_CONSTANT>.                    { return ConstantLexemeAction(createLexicalAnalyzerContext(), C_CHARACTER); }
<CHAR_CONSTANT>"'"                  { IgnoredLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); } */
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
"boolean"                           { return TypeLexemeAction(createLexicalAnalyzerContext(), V_BOOLEAN); } 

[0-9]+\.[0-9]+f						{ return ConstantLexemeAction(createLexicalAnalyzerContext(), C_FLOAT); }
[0-9]+\.[0-9]+						{ return ConstantLexemeAction(createLexicalAnalyzerContext(), C_DOUBLE); }
[0-9]+						        { return ConstantLexemeAction(createLexicalAnalyzerContext(), C_INTEGER); }

[[:alpha:]]+                        { return NameLexemeAction(createLexicalAnalyzerContext()); }
[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }
%%

#include "FlexExport.h"
