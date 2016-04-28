#include "TH_utility.h"
//#include "iconv/iconv.h"
#include "platform/CCStdC.h"

USING_NS_CC;

namespace TH
{
	unsigned int g_seed = 0;

	long getCurrentTime() 
	{     
		struct timeval tv;
		gettimeofday(&tv, NULL);  
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}

	void getSysTime(stSysTime &gTime)
	{
		struct tm *tm;
		time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		time(&timep);
#else
		struct timeval now;
		gettimeofday(&now,  0);
		timep = now.tv_sec;
#endif

		tm = localtime(&timep);
		gTime.year	= tm->tm_year + 1900;
		gTime.month = tm->tm_mon + 1;
		gTime.day	= tm->tm_mday;
		gTime.hour	= tm->tm_hour;
		gTime.min	= tm->tm_min;
		gTime.second= tm->tm_sec;
	}

	int analyInt( std::string &str, char type )
	{
		int id = str.find(type);
		if (id != -1)
		{
			__String val = str.substr(0, id);

			str = str.substr(id+1);

			return val.intValue();
		}
		return 0;
	}

	void random_Seed( int seed )
	{
		if(!seed) g_seed = (unsigned int)getCurrentTime();
		else g_seed = seed;
	}

	int random_Int( int min, int max )
	{
		g_seed=214013*g_seed+2531011;
		return min+(g_seed ^ g_seed>>15)%(max-min+1);
	}

	float random_Float( float min, float max )
	{
		g_seed=214013*g_seed+2531011;
		return min+(g_seed>>16)*(1.0f/65535.0f)*(max-min);
	}
}