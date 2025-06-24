#include "Generator.h"
#include "ExpressionGenerator.h"
#include "StructureGenerator.h"
#include <stdbool.h>
#include <sys/stat.h>
#include "JavaFileUtils.h"

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

static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);
static bool _ensureTestResultsDirectory(void);

static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

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
	
	size_t formatLen = strlen(format);
	if (formatLen > 0 && format[formatLen - 1] == '\n') {
		_atLineStart = true;
	} else {
		_atLineStart = false;
	}
	
	free(effectiveFormat);
	va_end(arguments);
}

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

bool writeGeneratedOutputToFile(CompilerState* compilerState, const char* testName) {
	if (!_ensureTestResultsDirectory()) {
		return false;
	}

	char* className = getMainClassName((Program*)compilerState->abstractSyntaxtTree);
	const char* fileBaseName = className ? className : testName;

	char* filename = concatenate(3, "test_results/", fileBaseName, ".java");
	_outputFile = fopen(filename, "w");
	free(filename);

	if (_outputFile == NULL) {
		logError(_logger, "Failed to open output file for test: %s", fileBaseName);
		return false;
	}

	_atLineStart = true;
	logDebugging(_logger, "Writing generated output to file for test: %s", fileBaseName);
	generateProgram(0, (Program*)compilerState->abstractSyntaxtTree);
	
	fclose(_outputFile);
	_outputFile = NULL;
	return true;
}

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_atLineStart = true; 
	generateProgram(0, (Program*)compilerState->abstractSyntaxtTree); 
	logDebugging(_logger, "Generation is done.");
}
