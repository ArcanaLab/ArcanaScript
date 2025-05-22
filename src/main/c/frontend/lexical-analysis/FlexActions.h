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

#pragma region Module Initialization
/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();
#pragma endregion

#pragma region Comment Handling
/**
 * @brief Handles actions for lexemes that should be ignored during lexical analysis.
 *
 * This function is called when the lexical analyzer encounters a lexeme that does not
 * require further processing or should be skipped (e.g., whitespace, comments).
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 */
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * @brief Begins the processing of a multiline comment lexeme in the lexical analyzer.
 *
 * This function updates the given lexical analyzer context to indicate that
 * a multiline comment lexeme has started. It typically adjusts the internal
 * state so that subsequent characters are treated as part of a multiline comment
 * until the comment is closed.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 */
void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * @brief Handles the end of a multiline comment lexeme during lexical analysis.
 *
 * This function is called when the lexical analyzer detects the end of a multiline comment.
 * It performs any necessary actions to finalize the processing of the comment and updates
 * the lexical analyzer context accordingly.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 */
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
#pragma endregion

#pragma region Literals and Constants
/**
 * Processes a constant lexeme during lexical analysis and returns the corresponding token.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token to be processed as a constant lexeme.
 * @return The processed token representing the constant lexeme.
 */
Token ConstantLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
#pragma endregion

#pragma region Operators
/**
 * @brief Processes an arithmetic operator lexeme during lexical analysis.
 *
 * This function handles the recognition and processing of arithmetic operator tokens
 * (such as '+', '-', '*', '/') within the lexical analyzer context.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token representing the arithmetic operator lexeme.
 * @return Token The processed token after handling the arithmetic operator.
 */
Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

/**
 * @brief Processes a comparator lexeme and returns the corresponding Token.
 *
 * This function analyzes the current state of the lexical analyzer context to identify
 * and process a comparator lexeme (such as ==, !=, <, >, <=, >=) from the input stream.
 * It constructs and returns a Token representing the identified comparator.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @return Token representing the identified comparator lexeme.
 */
Token ComparatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Handles the action for recognizing an assignment operator lexeme during lexical analysis.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token representing the current lexeme being analyzed.
 * @return A Token object representing the processed assignment operator.
 */
Token AssignmentOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

/**
 * Handles the lexeme action for unary operators during lexical analysis.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token representing the unary operator to process.
 * @return A Token object after processing the unary operator lexeme.
 */
Token UnaryOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
#pragma endregion

#pragma region Delimiters
/**
 * @brief Handles the lexeme action for parenthesis tokens during lexical analysis.
 *
 * This function processes a token identified as a parenthesis (either opening or closing)
 * within the lexical analyzer context. It may update the context or modify the token as needed
 * for further parsing stages.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token representing a parenthesis to be processed.
 * @return Token The processed token after applying the parenthesis lexeme action.
 */
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

/**
 * @brief Handles the lexeme action for brace tokens during lexical analysis.
 *
 * This function processes tokens that represent braces (e.g., '{', '}') in the source code.
 * It may update the lexical analyzer context or modify the token as needed for further analysis.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token representing a brace lexeme to be processed.
 * @return The processed token after applying the brace lexeme action.
 */
Token BraceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

/**
 * @brief Handles the lexeme action for a comma token during lexical analysis.
 *
 * This function is called when a comma (',') is encountered in the input stream.
 * It processes the comma character and returns a corresponding Token object.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @return Token representing the comma lexeme.
 */
Token CommaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * @brief Handles the action for recognizing a semicolon lexeme during lexical analysis.
 *
 * This function is called when a semicolon (';') is encountered in the input stream.
 * It creates and returns a Token representing the semicolon.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @return Token representing the semicolon lexeme.
 */
Token SemicolonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * @brief Handles the action for recognizing a colon (:) lexeme during lexical analysis.
 *
 * This function is called by the lexical analyzer when a colon character is encountered in the input stream.
 * It creates and returns a Token representing the colon lexeme.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @return Token representing the colon lexeme.
 */
Token ColonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
#pragma endregion

#pragma region String Handling
/**
 * @brief Handles the lexeme action for an apostrophe character in the lexical analyzer.
 *
 * This function is called when an apostrophe character is encountered during lexical analysis.
 * It processes the apostrophe lexeme according to the rules defined in the lexical analyzer context.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 */
void ApostropheLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * @brief Handles the lexeme quoting action during lexical analysis.
 *
 * This function processes the current lexeme in the lexical analyzer context,
 * applying quoting rules as required by the language specification.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 */
void QuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
#pragma endregion

#pragma region Language Constructs
/**
 * @brief Processes the current lexeme as a name token.
 *
 * This function analyzes the current lexeme in the given lexical analyzer context
 * and returns a Token representing a name (identifier) found during lexical analysis.
 *
 * @param lexicalAnalyzerContext Pointer to the lexical analyzer context containing the current state.
 * @return Token representing the identified name lexeme.
 */
Token NameLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * @brief Processes a lexeme and returns its corresponding token.
 *
 * This function analyzes the current lexeme in the given lexical analyzer context
 * and determines its token type based on the specified variable type.
 *
 * @param lexicalAnalyzerContext Pointer to the lexical analyzer context containing the current state.
 * @param token The variable type to be analyzed and converted into a token.
 * @return Token The resulting token after processing the lexeme.
 */
Token TypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, VariableType token);

/**
 * @brief Processes a class lexeme during lexical analysis.
 *
 * This function is called when a class-related lexeme is recognized by the lexical analyzer.
 * It performs the necessary actions to create and return a Token representing the class lexeme.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @return Token representing the identified class lexeme.
 */
Token ClassLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Processes a decorator lexeme during lexical analysis.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token to be processed as a decorator lexeme.
 * @return A Token representing the result of the decorator lexeme action.
 */
Token DecoratorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
#pragma endregion

#pragma region Control Structures
/**
 * @brief Processes a loop lexeme action within the lexical analyzer context.
 *
 * This function handles the specific action to be taken when a loop-related lexeme
 * is encountered during lexical analysis. It operates on the provided lexical analyzer
 * context and the current token, potentially modifying or returning a new token as needed.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token associated with the current lexeme.
 * @return Token The resulting token after processing the loop lexeme action.
 */
Token LoopLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

/**
 * Handles the lexeme action for conditional structures during lexical analysis.
 *
 * This function processes tokens that represent conditional structures (such as 'if', 'else', etc.)
 * within the given lexical analyzer context. It may modify the token or context as needed to
 * properly handle conditional constructs in the source code being analyzed.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token representing a potential conditional structure.
 * @return A Token object, possibly modified, after processing the conditional structure.
 */
Token ConditionalStructureLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

/**
 * @brief Returns a Token representing the current lexeme identified by the lexical analyzer.
 *
 * This function processes the current state of the lexical analyzer context and
 * constructs a Token object corresponding to the lexeme that has just been recognized.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @return Token representing the recognized lexeme.
 */
Token ReturnLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * @brief Processes the current lexeme and returns a Token.
 *
 * This function is typically called during lexical analysis to handle the current
 * lexeme identified by the lexical analyzer. It uses the provided context to
 * determine the appropriate token to return.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context,
 *        which contains state and information about the input being analyzed.
 * @return Token representing the processed lexeme.
 */
Token PassLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
#pragma endregion

#pragma region Imports
/**
 * @brief Handles the import lexeme action during lexical analysis.
 *
 * This function processes the current lexeme in the context of an import statement,
 * updating the lexical analyzer context as needed and returning the corresponding Token.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @return Token representing the processed import lexeme.
 */
Token ImportLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * @brief Handles the action of importing a file lexeme during lexical analysis.
 *
 * This function processes the current lexeme as an import file directive within the lexical analyzer context.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @return Token representing the result of the import file lexeme action.
 */
Token ImportFileLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
#pragma endregion

#pragma region Utility
/**
 * Handles the action to be taken when an unknown or unrecognized lexeme is encountered
 * during lexical analysis.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @return A Token representing the unknown lexeme.
 */
Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * @brief Performs an action on a given token during lexical analysis, potentially extending its lexeme.
 *
 * This function is intended to be used within the lexical analyzer to process a token and possibly
 * extend its lexeme based on the current context.
 *
 * @param lexicalAnalyzerContext Pointer to the current lexical analyzer context.
 * @param token The token to be processed and potentially extended.
 * @return The resulting Token after applying the extension action.
 */
Token ExtensionLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
#pragma endregion

#endif