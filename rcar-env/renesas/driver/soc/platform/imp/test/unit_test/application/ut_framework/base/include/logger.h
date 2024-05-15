#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

enum LogCategory {
	LogCategoryValidator,
	LogCategoryCL,
	LogCategoryArgs,
	LogCategoryRaw,
	LogCategoryDebug,
	LogCategoryInfo,
	LogCategoryWarn,
	LogCategoryError,
	LogCategoryIn,
	LogCategoryOut,
	LogCategoryCount
};

enum LogMode {
	LogModeConsole,
	LogModeFile,
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void TEST_logInit();
void TEST_logPrint(enum LogCategory category, const char *file, const int32_t line, const char *format, ...);
bool TEST_isLogEnabled(enum LogCategory category);
void TEST_SetLogColorEnabled(bool enabled);

#define FLOG(format, ...)	TEST_logPrint(LogCategoryInfo,  	__FILE__, __LINE__, format, __VA_ARGS__)
#ifndef FULL_LOG
#define VLOG(format, ...)	TEST_logPrint(LogCategoryValidator,	__FILE__, __LINE__, format, __VA_ARGS__)
#define CLOG(format, ...)	TEST_logPrint(LogCategoryCL,    	__FILE__, __LINE__, format, __VA_ARGS__)
#define ALOG(format, ...)	TEST_logPrint(LogCategoryArgs,  	__FILE__, __LINE__, format, __VA_ARGS__)
#define RLOG(format, ...)	TEST_logPrint(LogCategoryRaw,  		__FILE__, __LINE__, format, __VA_ARGS__)
#define DLOG(format, ...)	TEST_logPrint(LogCategoryDebug, 	__FILE__, __LINE__, format, __VA_ARGS__)
#define ILOG(format, ...)	TEST_logPrint(LogCategoryInfo,  	__FILE__, __LINE__, format, __VA_ARGS__)
#define WLOG(format, ...)	TEST_logPrint(LogCategoryWarn,  	__FILE__, __LINE__, format, __VA_ARGS__)
#define ELOG(format, ...)	TEST_logPrint(LogCategoryError, 	__FILE__, __LINE__, format, __VA_ARGS__)
#define INLOG(format, ...)	TEST_logPrint(LogCategoryIn, 		__FILE__, __LINE__, format, __VA_ARGS__)
#define OUTLOG(format, ...)	TEST_logPrint(LogCategoryOut, 		__FILE__, __LINE__, format, __VA_ARGS__)
#else
#define VLOG(...)
#define CLOG(...)
#define ALOG(...)
#define RLOG(...)
#define DLOG(...)
#define ILOG(...)
#define WLOG(...)
#define ELOG(...)
#define INLOG(...)
#define OUTLOG(...)
#endif

#ifdef __cplusplus
}
#endif

#endif
