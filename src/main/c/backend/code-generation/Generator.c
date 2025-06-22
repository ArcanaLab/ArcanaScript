#include "Generator.h"
#include "ExpressionGenerator.h"
#include "StructureGenerator.h"
#include <stdbool.h>
#include <sys/stat.h>

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
static FILE* _outputFile = NULL;
static bool _atLineStart = true;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
	if (_outputFile != NULL) {
		fclose(_outputFile);
		_outputFile = NULL;
	}
}

/** PRIVATE FUNCTIONS */

static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);
static bool _ensureTestResultsDirectory(void);

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
	
	if (_outputFile != NULL) {
		vfprintf(_outputFile, effectiveFormat, arguments);
		fflush(_outputFile);
	} else {
		vfprintf(stdout, effectiveFormat, arguments);
		fflush(stdout);
	}
	
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/**
 * Public wrapper for the output function that other modules can use.
 */
void generatorOutput(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	
	char * effectiveFormat;
	if (_atLineStart) {
		char * indentation = _indentation(indentationLevel);
		effectiveFormat = concatenate(2, indentation, format);
		free(indentation);
	} else {
		effectiveFormat = concatenate(1, format);
	}
	
	if (_outputFile != NULL) {
		vfprintf(_outputFile, effectiveFormat, arguments);
		fflush(_outputFile);
	} else {
		vfprintf(stdout, effectiveFormat, arguments);
		fflush(stdout);
	}
	
	// Check if the format ends with a newline to track line starts
	size_t formatLen = strlen(format);
	if (formatLen > 0 && format[formatLen - 1] == '\n') {
		_atLineStart = true;
	} else {
		_atLineStart = false;
	}
	
	free(effectiveFormat);
	va_end(arguments);
}

/**
 * Ensures the test_results directory exists.
 * @return true if the directory exists or was created successfully, false otherwise
 */
static bool _ensureTestResultsDirectory(void) {
	struct stat st = {0};
	if (stat("test_results", &st) == -1) {
		#ifdef _WIN32
			if (mkdir("test_results") == -1) {
				logError(_logger, "Failed to create test_results directory");
				return false;
			}
		#else
			if (mkdir("test_results", 0700) == -1) {
				logError(_logger, "Failed to create test_results directory");
				return false;
			}
		#endif
	}
	return true;
}

/** PUBLIC FUNCTIONS */

bool writeGeneratedOutputToFile(CompilerState* compilerState, const char* testName) {
	if (!_ensureTestResultsDirectory()) {
		return false;
	}

	char* filename = concatenate(3, "test_results/", testName, ".java");
	_outputFile = fopen(filename, "w");
	free(filename);

	if (_outputFile == NULL) {
		logError(_logger, "Failed to open output file for test: %s", testName);
		return false;
	}

	_atLineStart = true; // Reset line start flag for new file
	logDebugging(_logger, "Writing generated output to file for test: %s", testName);
	generateProgram(0, (Program*)compilerState->abstractSyntaxtTree);
	
	fclose(_outputFile);
	_outputFile = NULL;
	return true;
}

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_atLineStart = true; // Reset line start flag
	generateProgram(0, (Program*)compilerState->abstractSyntaxtTree); //ESTE CASTEO ES DUDOSISIMO AYUDAME LOCO
	logDebugging(_logger, "Generation is done.");
}
