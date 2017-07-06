// DiceLog.h : Log utility class
//

#ifndef __DICELOG_H__
#define __DICELOG_H__

#include <sstream>
#include <string>
#include <stdio.h>
#include <windows.h>

inline std::string NowTime();

enum TLogLevel { logERROR, logWARNING, logINFO, logVERBOSE };

class Log
{
public:
	Log();
	virtual ~Log();
	std::ostringstream& Get(TLogLevel level = logINFO);
public:
	static TLogLevel& ReportingLevel();
	static std::string ToString(TLogLevel level);
	static TLogLevel FromString(const std::string& level);
protected:
	std::ostringstream os;
private:
	Log(const Log&);
	Log& operator =(const Log&);
};

inline Log::Log()
{
}

inline std::ostringstream& Log::Get(TLogLevel level)
{
	os << "- " << NowTime();
	os << " " << ToString(level) << ": ";
	os << std::string(level > logVERBOSE ? level - logVERBOSE : 0, '\t');
	return os;
}

inline Log::~Log()
{
	os << std::endl;
	OutputDebugStringA(os.str().c_str());
	fprintf(stderr, "%s", os.str().c_str());
	fflush(stderr);
}

inline TLogLevel& Log::ReportingLevel()
{
	static TLogLevel reportingLevel = logINFO;
	return reportingLevel;
}

inline std::string Log::ToString(TLogLevel level)
{
	static const char* const buffer[] = { "ERROR", "WARNING", "INFO", "VERBOSE"};
	return buffer[level];
}

inline TLogLevel Log::FromString(const std::string& level)
{
	if (level == "VERBOSE")
		return logVERBOSE;
	if (level == "INFO")
		return logINFO;
	if (level == "WARNING")
		return logWARNING;
	if (level == "ERROR")
		return logERROR;
	Log().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
	return logINFO;
}

typedef Log FILELog;

#define DICE_LOG_INTERNAL(level) \
    if (level > FILELog::ReportingLevel()) ; \
    else Log().Get(level)

// Helpers

#define DICE_LOG_ERROR(message) \
	DICE_LOG_INTERNAL(logERROR) << message;

#define DICE_LOG_WARNING(message) \
	DICE_LOG_INTERNAL(logWARNING) << message;

#define DICE_LOG_INFO(message) \
	DICE_LOG_INTERNAL(logINFO) << message;

#define DICE_LOG_VERBOSE(message) \
	DICE_LOG_INTERNAL(logVERBOSE) << message;

inline std::string NowTime()
{
	const int MAX_LEN = 200;
	char buffer[MAX_LEN];
	if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0,
		"HH':'mm':'ss", buffer, MAX_LEN) == 0)
		return "Error in NowTime()";

	char result[100] = { 0 };
	static DWORD first = GetTickCount();
	sprintf_s(result, "%s.%03ld", buffer, (long)(GetTickCount() - first) % 1000);
	return result;
}



#endif //__DICELOG_H__

