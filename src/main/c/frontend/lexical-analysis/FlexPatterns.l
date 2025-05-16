%{
    #include "FlexActions.h"
    #include "../../shared/Logger.h"

    
%}

%option stack

/**
 * Lexical patterns for type keywords and identifiers.
 * 
 * @see https://westes.github.io/flex/manual/
 */

%%
":"         {  return COLON; }
";"         {  return SEMICOLON; }

"char"      { yylval.varType = CHAR_TYPE; return TYPE; }
"int"       { yylval.varType = INT_TYPE; return TYPE; }
    /**
    "float"     { return FloatLexemeAction(createLexicalAnalyzerContext()); }
    "boolean"   { return BooleanLexemeAction(createLexicalAnalyzerContext()); }
    "long"      { return LongLexemeAction(createLexicalAnalyzerContext()); }
    "short"     { return ShortLexemeAction(createLexicalAnalyzerContext()); }
    "double"    { return DoubleLexemeAction(createLexicalAnalyzerContext()); }
    */

[[:alnum:]]+    { return IdentifierLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+    { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.               { return UnknownLexemeAction(createLexicalAnalyzerContext()); }



%%

#include "FlexExport.h"