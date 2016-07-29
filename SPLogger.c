#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

typedef enum spLoggerSeverity
{
	SP_LOGGER_ERROR_SEVERITY, //Error level
	SP_LOGGER_WARNING_SEVERITY, //Warning level
	SP_LOGGER_INFO_SEVERITY, //Info level
	SP_LOGGER_DEBUG_SEVERITY //Debug level
} spLoggerSeverity;

char* severity2String[4] = {"---ERROR---", "---WARNING---", "---INFO---", "---DEBUG---"};

SP_LOGGER_MSG spLoggerPrintFunc(const char* msg, const char* file, const char* function, const int line, spLoggerSeverity severity)
{
	if (NULL == logger)
	{ //Already defined
		return SP_LOGGER_UNDIFINED;
	}

	if (NULL == msg || NULL == file || NULL == function || 0 > line)
	{
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if((int)severity <= (int)logger->level)
	{
		int rc_code[5] = {0,0,0,0,0};
		if((int)severity==2){
			rc_code[0] = fprintf(logger->outputChannel, "%s\n", severity2String[severity]);
			rc_code[4] = fprintf(logger->outputChannel, "- message: %s\n", msg);
		}
		else{
			rc_code[0] = fprintf(logger->outputChannel, "%s\n", severity2String[severity]);
			rc_code[1] = fprintf(logger->outputChannel, "- file: %s\n", file);
			rc_code[2] = fprintf(logger->outputChannel, "- function: %s\n", function);
			rc_code[3] = fprintf(logger->outputChannel, "- line: %d\n", line);
			rc_code[4] = fprintf(logger->outputChannel, "- message: %s\n", msg);
		}

		int i;
		for (i=0; i<4; i++)
		{
			if (0 > rc_code[i])
			{
				return SP_LOGGER_WRITE_FAIL;
			}
		}
	}
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}

SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line)
{
	return spLoggerPrintFunc(msg, file, function, line, SP_LOGGER_ERROR_SEVERITY);
}

SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line)
{
	return spLoggerPrintFunc(msg, file, function, line, SP_LOGGER_WARNING_SEVERITY);
}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg)
{
	return spLoggerPrintFunc(msg, "a", "a", 1, SP_LOGGER_INFO_SEVERITY);
}

SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line)
{
	return spLoggerPrintFunc(msg, file, function, line, SP_LOGGER_DEBUG_SEVERITY);
}

SP_LOGGER_MSG spLoggerPrintMsg(const char* msg)
{
	if (NULL == logger)
	{ //Already defined
		return SP_LOGGER_UNDIFINED;
	}

	if (NULL == msg)
	{
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if (0 > fprintf(logger->outputChannel, "%s\n", msg))
	{
		return SP_LOGGER_WRITE_FAIL;
	}

	return SP_LOGGER_SUCCESS;
}
