#include <stdarg.h>

#include "testenv.h"
#include "logger.h"
#include "file.h"

#if 1
static const char bold_red[]	= "\0";

static const char black[]	= "\0";
static const char red[]		= "\0";
static const char green[]   = "\0";
static const char yellow[]  = "\0";
static const char blue[]    = "\0";
static const char magenta[] = "\0";
static const char cyan[]    = "\0";
static const char white[]   = "\0";
static const char reset[]   = "\0";
#else
static const char bold_red[]	= "\e[1;31m";

static const char black[]	= "\x1b[30m";
static const char red[]		= "\x1b[31m";
static const char green[]   = "\x1b[32m";
static const char yellow[]  = "\x1b[33m";
static const char blue[]    = "\x1b[34m";
static const char magenta[] = "\x1b[35m";
static const char cyan[]    = "\x1b[36m";
static const char white[]   = "\x1b[37m";
static const char reset[]   = "\x1b[0m";
#endif

static bool m_logInitialized = false;
static enum LogMode m_logMode = LogModeConsole;
static bool m_logStyleEnabled = true;

static bool m_logEnabled[LogCategoryCount] =
{
#if !defined(__WINAMS__)
	true, 		// LogCategoryValidator
	true, 		// LogCategoryCL
	true, 		// LogCategoryArgs
	true, 		// LogCategoryRaw
	true, 		// LogCategoryDebug
	true, 		// LogCategoryInfo
	true, 		// LogCategoryWarn
	true, 		// LogCategoryError
	true,		// LogCategoryIn,
	true,		// LogCategoryOut,
#else
	false, 		// LogCategoryValidator
	false, 		// LogCategoryCL
	false, 		// LogCategoryArgs
	false, 		// LogCategoryRaw
	false, 		// LogCategoryDebug
	false, 		// LogCategoryInfo
	false, 		// LogCategoryWarn
	false, 		// LogCategoryError
	false,		// LogCategoryIn,
	false,		// LogCategoryOut,
#endif
};

static const char *m_logStyle[LogCategoryCount] =
{
	green, 		// LogCategoryValidator
	cyan, 		// LogCategoryCL
	magenta, 	// LogCategoryArgs
	white, 		// LogCategoryRaw
	white, 		// LogCategoryDebug
	white, 		// LogCategoryInfo
	yellow, 	// LogCategoryWarn
	bold_red, 	// LogCategoryError
	magenta,	// LogCategoryIn,
	magenta,	// LogCategoryOut,
};

static void nowTimeString(char *str)
{
#if 0
    std::string str;
	char s[32];

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();   // ç�¾åœ¨æ™‚åˆ»ã‚’å�–å¾—
    std::chrono::system_clock::duration tp = now.time_since_epoch();                // çµŒé�Žç§’æ•°ã‚’å�–å¾—

    // mséƒ¨åˆ†ã‚’å�–å¾—
    tp -= std::chrono::duration_cast<std::chrono::seconds>(tp);

    // ç�¾åœ¨æ™‚åˆ»ã‚’time_tåž‹ã�«å¤‰æ�›
    time_t tt = std::chrono::system_clock::to_time_t(now);

    DateTime dt = DateTimeUtility::toApplicationTime(tt);

    // æ–‡å­—åˆ—ã‚’ä½œæˆ�
    memset(&s, 0, sizeof(s));
    sprintf(s, "%04d/%02d/%02d %02d:%02d:%02d.%03d", dt.date.year, dt.date.month, dt.date.day,
                                                     dt.time.hour, dt.time.minute, dt.time.second, (int)(tp / std::chrono::milliseconds(1)));
	str = s;
#endif
}


void TEST_logInit()
{

}

void TEST_logPrint(enum LogCategory category, const char *file, const int32_t line, const char *format, ...)
{
    const char* prefix[LogCategoryCount] = { "[V]", "[C]", "[A]", "", "[D]", "[I]", "[W]", "[E]", "[I N]", "[OUT]" };

    if (!m_logInitialized && (m_logMode == LogModeFile)) {
        m_logInitialized = true;
    }

	if (TEST_isLogEnabled(category)) {

		char nowTimeStr[64] = {0};
		nowTimeString(nowTimeStr);

		FILE *fp = (m_logMode == LogModeFile) ? TEST_FOPEN("log.txt", "a") : stdout;

		if (fp) {
			va_list args;
			va_start(args, format);

			switch (category) {
			case LogCategoryCL:
			{
				if (m_logStyleEnabled)
				{
//                    fprintf(fp, "%s [%s]<%s %s:%d> ", timeStr, categoryCharactor[category], file, line);
					TEST_FPRINTF(fp, "%s%s%s", m_logStyle[category], nowTimeStr, prefix[category]);
					TEST_VFPRINTF(fp, format, args);
					TEST_FPRINTF(fp, "%s\n", reset);
				}
				else
				{
					TEST_FPRINTF(fp, "%s%s", nowTimeStr, prefix[category]);
					TEST_VFPRINTF(fp, format, args);
					TEST_FPRINTF(fp, "\n");
				}
			}
			break;
			case LogCategoryValidator:
			case LogCategoryArgs:
			case LogCategoryRaw:
			case LogCategoryDebug:
			case LogCategoryInfo:
			case LogCategoryError:
			case LogCategoryWarn:
			case LogCategoryIn:
			case LogCategoryOut:
				{
					if (m_logStyleEnabled)
					{
//                    fprintf(fp, "%s [%s]<%s %s:%d> ", timeStr, categoryCharactor[category], file, line);
						TEST_FPRINTF(fp, "%s%s%s", m_logStyle[category], nowTimeStr, prefix[category]);
						TEST_VFPRINTF(fp, format, args);
						TEST_FPRINTF(fp, "%s\n", reset);
					}
					else
					{
						TEST_FPRINTF(fp, "%s%s", nowTimeStr, prefix[category]);
						TEST_VFPRINTF(fp, format, args);
						TEST_FPRINTF(fp, "\n");
					}
				}
				break;
			default:
				break;
			}

			va_end(args);
        }

        if (m_logMode == LogModeFile) {
        	TEST_FCLOSE(fp);
        }
    }
}

bool TEST_isLogEnabled(enum LogCategory category)
{
	return m_logEnabled[category];
}

void TEST_SetLogColorEnabled(bool enabled)
{
	m_logStyleEnabled = enabled;
}

