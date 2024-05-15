#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "logger.h"

#if defined(__CYGWIN__) || defined(QNX_OS) || defined(LINUX_OS)
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#elif defined(WIN32) || defined(WINDOWS_OS) 
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <direct.h>

#define S_IRWXU		(0)
#define S_IRGRP		(1)
#define S_IXGRP		(2)
#define S_IROTH		(4)
#define S_IXOTH		(8)
#define mkdir(a, b)	_mkdir(a)
#endif

struct SubDirInfo
{
	const char *api;
	const char *subdir;
	int32_t subset_count;
	int32_t cl_pattern_count;
};

struct SUBSET_TO_STRING_TABLE {
	uint32_t subset;
	const char *name;
};

#define ENUM_TO_STRING(a) { (uint32_t)a, #a }

static char m_expectedDirPath[512] = {0};
static char m_outputDirPath[512] = "./output";
static bool m_outputTimestampEnabled = true;

#if defined(__CYGWIN__) || defined(WINDOWS_OS) || defined(QNX_OS) || defined(LINUX_OS)
static void TEST_File_CreateDir(const char *dir)
{
	char tmp[512];
	char *p = NULL;
	size_t len;
	uint32_t mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

	snprintf(tmp, sizeof(tmp),"%s",dir);
	len = strlen(tmp);
	if (tmp[len - 1] == '/')
	{
		tmp[len - 1] = 0;
	}

	for (p = tmp + 1; *p; p++)
	{
		if (*p == '/') {
			*p = 0;
			mkdir(tmp, mode);
			*p = '/';
		}
	}

	mkdir(tmp, mode);
}
#endif

void TEST_File_SetExpectedDirPath(const char *path)
{
	strncpy(m_expectedDirPath, path, sizeof(m_expectedDirPath));
}

void TEST_File_SetOutputDirPath(const char *path)
{
	strncpy(m_outputDirPath, path, sizeof(m_outputDirPath));
}

void TEST_File_SetCLOutputMode(enum TEST_CLOutputMode mode)
{
}

void TEST_File_ClearCLOutputMode()
{
}

void TEST_File_SetOutputTimestampEnabled(bool enabled)
{
	m_outputTimestampEnabled = enabled;
}

void TEST_File_CoverageCL(const struct TestParams *params)
{
}

void TEST_File_PrintCLCoverage()
{
	return;
}

void TEST_File_Init()
{
#if defined(__CYGWIN__) || defined(WINDOWS_OS) || defined(QNX_OS) || defined(LINUX_OS)
	char path[512] = {0};

	if (m_outputTimestampEnabled)
	{
		time_t timer;
		struct tm *t_st;

		time(&timer);
		t_st = localtime(&timer);

		sprintf(path, "/result_%d%02d%02d_%02d%02d%02d",
				t_st->tm_year+1900, t_st->tm_mon+1, t_st->tm_mday,
				t_st->tm_hour, t_st->tm_min, t_st->tm_sec);
	}
	else
	{
		strcpy(path, "/result");
	}

	strcat(m_outputDirPath, path);
	TEST_File_CreateDir(m_outputDirPath);

#endif
}

void TEST_File_ChangeDir(const char *dir)
{
#if defined(__CYGWIN__) || defined(WINDOWS_OS) || defined(QNX_OS) || defined(LINUX_OS)
	chdir(dir);
#endif
}

uint32_t *TEST_File_ExpectedData(const struct TestParams *params)
{
	return NULL;
}

void TEST_File_FPrintCL(
	const struct TestParams * const params
	)
{
	return;
}


