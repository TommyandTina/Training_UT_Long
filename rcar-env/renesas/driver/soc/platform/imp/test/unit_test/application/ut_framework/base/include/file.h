#ifndef FILE_H_
#define FILE_H_

#include <stdint.h>
#include <stdbool.h>

#include "params.h"

#define TEST_STDOUT							(stdout)

#if defined(__CYGWIN__) || defined(WIN32)
#define TEST_FPRINTF(...)					(fprintf(__VA_ARGS__))
#define TEST_SPRINTF(...)					(sprintf(__VA_ARGS__))
#define TEST_VFPRINTF(...)					(vfprintf(__VA_ARGS__))

#define TEST_FOPEN(arg1, arg2)				(fopen(arg1, arg2))
#define TEST_FCLOSE(arg1)					(fclose(arg1))
#define TEST_FREAD(arg1, arg2, arg3, arg4)	(fread(arg1, arg2, arg3, arg4))
#define TEST_FWRITE(arg1, arg2, arg3, arg4)	(fwrite(arg1, arg2, arg3, arg4))
#define TEST_FSEEK(arg1, arg2, arg3)		(fseek(arg1, arg2, arg3))
#define TEST_FTELL(arg1)					(ftell(arg1))
#define TEST_FGETS(arg1, arg2, arg3)		(fgets(arg1, arg2, arg3))

#elif __WINAMS__

#define TEST_FPRINTF(...)
#define TEST_SPRINTF(...)
#define TEST_VFPRINTF(...)

#define TEST_FOPEN(arg1, arg2)                  (NULL)
#define TEST_FCLOSE(arg1)
#define TEST_FREAD(arg1, arg2, arg3, arg4)
#define TEST_FWRITE(arg1, arg2, arg3, arg4)
#define TEST_FSEEK(arg1, arg2, arg3)
#define TEST_FTELL(arg1)
#define TEST_FGETS(arg1, arg2, arg3)

#else

#define TEST_FPRINTF(...)					(fprintf(__VA_ARGS__))
#define TEST_SPRINTF(...)					(sprintf(__VA_ARGS__))
#define TEST_VFPRINTF(...)					(vfprintf(__VA_ARGS__))

#define TEST_FOPEN(arg1, arg2)                  (NULL)
#define TEST_FCLOSE(arg1)
#define TEST_FREAD(arg1, arg2, arg3, arg4)
#define TEST_FWRITE(arg1, arg2, arg3, arg4)
#define TEST_FSEEK(arg1, arg2, arg3)
#define TEST_FTELL(arg1)
#define TEST_FGETS(arg1, arg2, arg3)

#endif  /* __CYGWIN__ */



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum TEST_CLOutputMode {
	CLOutputToConsole = 0,
	CLOutputToTextFile,
	CLOutputToBinaryFile
};

/* FILE PRINT */
void TEST_File_Init();
void TEST_File_SetExpectedDirPath(const char *path);
void TEST_File_SetOutputDirPath(const char *path);
void TEST_File_SetCLOutputMode(enum TEST_CLOutputMode mode);
void TEST_File_ClearCLOutputMode();
void TEST_File_SetOutputTimestampEnabled(bool enabled);
uint32_t *TEST_File_ExpectedData(const struct TestParams *params);
void TEST_File_FPrintCL(const struct TestParams * const testparams);
void TEST_File_CoverageCL(const struct TestParams *params);
void TEST_File_PrintCLCoverage();

#ifdef __cplusplus
}
#endif

#endif /* FILE_H_ */
