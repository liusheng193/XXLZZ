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
	// �����ռ�
	bool log_Create(const char* file, int id);

	// ���ռ��ļ�(�����ڽ��ᴴ��)
	bool log_Open(const char* file, int id);
	// �ر�
	void log_Close(int id);

	// д���ռ�
	void log_Write(int id, bool bTime, const char* log, ...);
	void log_WriteTp(int id, bool bTime, int tp, const char* log, ...);
}

#define THLog_Write	TH::log_Write
#define THLog_WriteTp	TH::log_Write

#endif