#ifndef  __TH_LOG_H__
#define  __TH_LOG_H__

enum
{
	LOG_NOTE,
	LOG_WARNING,
	LOG_ERROR,
};

// log
namespace TH
{
	// 创建日记
	bool log_Create(const char* file, int id);

	// 打开日记文件(不存在将会创建)
	bool log_Open(const char* file, int id);
	// 关闭
	void log_Close(int id);

	// 写入日记
	void log_Write(int id, bool bTime, const char* log, ...);
	void log_WriteTp(int id, bool bTime, int tp, const char* log, ...);
}

#define THLog_Write	TH::log_Write
#define THLog_WriteTp	TH::log_Write

#endif