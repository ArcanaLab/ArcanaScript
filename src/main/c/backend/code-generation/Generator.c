#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static const char _expressionTypeToCharacter(const ExpressionType type);
static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
static void _generateEpilogue(const int value);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generateInstruction(const unsigned int identationLevel, Instruction * instruction);
static void _generateBlock(const unsigned int identationLevel, Block * block);
static void _generateProgram(Program * program);


static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

/**
 * Converts and expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */
static const char _expressionTypeToCharacter(const ExpressionType type) {
	switch (type) {
		case ADDITION: return '+';
		case DIVISION: return '/';
		case MULTIPLICATION: return '*';
		case SUBTRACTION: return '-';
		default:
			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
			return '\0';
	}
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
// static void _generateEpilogue(const int value) {
// 	_output(0, "%s%d%s",
// 		"            [ $", value, "$, circle, draw, blue ]\n"
// 		"        ]\n"
// 		"    \\end{forest}\n"
// 		"\\end{document}\n\n"
// 	);
// }

/**
 * Generates the output of an expression.
 */
static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
	switch (expression->type) {
		case ADDITION:
		case DIVISION:
		case MULTIPLICATION:
		case SUBTRACTION:
			_generateExpression(1 + indentationLevel, expression->leftExpression);
			_output(1 + indentationLevel, "%c");
			_generateExpression(1 + indentationLevel, expression->rightExpression);
			break;
		case FACTOR:
			_generateFactor(1 + indentationLevel, expression->factor);
			break;
		default:
			logError(_logger, "The specified expression type is unknown: %d", expression->type);
			break;
	}
}



/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {

	_generateBlock(3, program->block);
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
// static void _generatePrologue(void) {
// 	_output(0, "%s",
// 		"\\documentclass{standalone}\n\n"
// 		"\\usepackage[utf8]{inputenc}\n"
// 		"\\usepackage[T1]{fontenc}\n"
// 		"\\usepackage{amsmath}\n"
// 		"\\usepackage{forest}\n"
// 		"\\usepackage{microtype}\n\n"
// 		"\\begin{document}\n"
// 		"    \\centering\n"
// 		"    \\begin{forest}\n"
// 		"        [ \\text{$=$}, circle, draw, purple\n"
// 	);
// }

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}


/**
 * Generates the output of a constant.
 */
static void _generateConstant(const unsigned int indentationLevel, Constant * constant) {
	switch (constant->type)
    {
    case C_INT_TYPE:
        _output(indentationLevel, "%d", constant->intValue);
        break;
    case C_CHAR_TYPE:
        _output(indentationLevel, "'%c'", constant->charValue);
        break;
    case C_BOOLEAN_TYPE:
        _output(indentationLevel, "%s", constant->booleanValue ? "true" : "false");
        break;
    case C_STRING_TYPE:
        _output(indentationLevel, "\"%s\"", constant->stringValue);
        break;
    case C_DOUBLE_TYPE:
        _output(indentationLevel, "%lf", constant->doubleValue);
        break;
    case C_FLOAT_TYPE:
        _output(indentationLevel, "%f", constant->floatValue);
        break;
    default:
        logError(_logger, "Constant type %d does not exists", constant->type);
		exit(140);
		break;
        
    }
}

/**
 * Generates the output of a factor.
 */
static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
	switch (factor->type) {
		case CONSTANT:
			_generateConstant(1 + indentationLevel, factor->constant);
			break;
		case EXPRESSION:
			_generateExpression(1 + indentationLevel, factor->expression);
			break;
		default:
			logError(_logger, "The specified factor type is unknown: %d", factor->type);
			break;
	}
}

static void _generateInstruction(const unsigned int identationLevel, Instruction * instruction)
{
    switch (instruction->type){
		case INSTRUCTION_EXPRESSION:
			_generateExpression(identationLevel + 1, instruction->expression);
			break;
		default:
			logError(_logger, "Instruction type %d does not exists", instruction->type);
			exit(140);
			break;
	}
}

static void _generateBlock (const unsigned int identationLevel, Block * block) {
	InstructionNode * currentInstruction = block->first;
	while(currentInstruction != NULL) {
		_generateInstruction(identationLevel + 1, (Instruction *) currentInstruction->data);
		currentInstruction = currentInstruction->next;
	}
}

/** PUBLIC FUNCTIONS */
void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	// _generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	// _generateEpilogue(compilerState->value);
	logDebugging(_logger, "Generation is done.");
}
