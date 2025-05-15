%{
    #include "FlexActions.h"
%}

%option stack

/**
 * Lexical patterns for type keywords and identifiers.
 * 
 * @see https://westes.github.io/flex/manual/
 */

%%

"char"      { return CharLexemeAction(createLexicalAnalyzerContext()); }
"int"       { return IntegerLexemeAction(createLexicalAnalyzerContext()); }
"float"     { return FloatLexemeAction(createLexicalAnalyzerContext()); }
"boolean"   { return BooleanLexemeAction(createLexicalAnalyzerContext()); }
"long"      { return LongLexemeAction(createLexicalAnalyzerContext()); }
"short"     { return ShortLexemeAction(createLexicalAnalyzerContext()); }
"double"    { return DoubleLexemeAction(createLexicalAnalyzerContext()); }

[[:alnum:]]+    { return IdentifierLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+    { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.               { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"