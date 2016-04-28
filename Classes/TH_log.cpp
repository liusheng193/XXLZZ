#include "TH_log.h"
#include "cocos2d.h"
#include <stdio.h>
#include "TH_utility.h"

#define MAX_LEN         (cocos2d::MAX_LOG_LENGTH + 1)

namespace TH
{
	struct stLogFile 
	{
		std::string file;
		FILE		*fp;
	};
	typedef std::map<int, stLogFile> MapLogFile;
	static MapLogFile g_mapLogFile;

	bool log_Create( const char* file, int id )
	{
		MapLogFile::iterator iter = g_mapLogFile.find(id);
		if (iter != g_mapLogFile.end())
		{
			return true;
		}
		stLogFile log;
		log.file = cocos2d::CCFileUtils::getInstance()->fullPathForFilename(file);
		log.fp = nullptr;

		// open
		FILE *fp = fopen(log.file.c_str(), "w");
		if (fp == nullptr)
		{
			return false;
		}
		fclose(fp);
		
		g_mapLogFile[id] = log;
		return true;
	}

	bool log_Open( const char* file, int id )
	{
		MapLogFile::iterator iter = g_mapLogFile.find(id);
		if (iter != g_mapLogFile.end())
		{
			return true;
		}

		stLogFile log;
		log.file = cocos2d::CCFileUtils::getInstance()->fullPathForFilename(file);
		log.fp = nullptr;

		// open
		FILE *fp = fopen(log.file.c_str(), "w");
		if (fp == nullptr)
		{
			return false;
		}
		// fclose(fp);
		log.fp = fp;

		g_mapLogFile[id] = log;
		return true;
	}

	void log_Close( int id )
	{
		MapLogFile::iterator iter = g_mapLogFile.find(id);
		if (iter != g_mapLogFile.end())
		{
			if (iter->second.fp) fclose(iter->second.fp);
			iter->second.fp = nullptr;
			g_mapLogFile.erase(iter);
		}
	}

	void log_Write( int id, bool bTime, const char* log, ... )
	{
		MapLogFile::iterator iter = g_mapLogFile.find(id);
		if (iter != g_mapLogFile.end())
		{
			char buf[MAX_LEN];

			va_list args;
			va_start(args, log);
			vsnprintf(buf, MAX_LEN, log, args);
			va_end(args);

			bool bClose = false;
			FILE *fp = iter->second.fp;
			if (fp == nullptr)
			{
				fp = fopen(iter->second.file.c_str(), "a");
				bClose = true;
			}

			// time
			if (!bTime)
			{
				fprintf(fp, "%s\n", buf);
			}
			else 
			{
				TH::stSysTime tm;
				TH::getSysTime(tm);
				fprintf(fp, "[%d-%d-%d %d:%d:%d] %s\n", tm.year, tm.month, tm.day, tm.hour, tm.min, tm.second, buf);
			}

			if (bClose && fp) fclose(fp);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			printf("%s\n", buf);
#endif
		}
	}

	void log_WriteTp( int id, bool bTime, int tp, const char* log, ... )
	{
		MapLogFile::iterator iter = g_mapLogFile.find(id);
		if (iter != g_mapLogFile.end())
		{
			char buf[MAX_LEN];

			va_list args;
			va_start(args, log);
			vsnprintf(buf, MAX_LEN, log, args);
			va_end(args);

			bool bClose = false;
			FILE *fp = iter->second.fp;
			if (fp == nullptr)
			{
				fp = fopen(iter->second.file.c_str(), "a");
				bClose = true;
			}

			const char* type = "";
			switch (tp)
			{
			case LOG_WARNING: type = "[Warning] ";
				break;
			case LOG_ERROR: type = "[Error] ";
				break;
			}

			// time
			if (!bTime)
			{
				fprintf(fp, "%s%s\n", type, buf);
			}
			else 
			{
				TH::stSysTime tm;
				TH::getSysTime(tm);
				fprintf(fp, "[%d-%d-%d %d:%d:%d] %s%s\n", tm.year, tm.month, tm.day, tm.hour, tm.min, tm.second, type, buf);
			}

			if (bClose && fp) fclose(fp);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			printf("%s%s\n", type, buf);
#endif
		}
	}

}