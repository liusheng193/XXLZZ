#include "TH_crash.h"

//#include "CCPlatformConfig.h"
#include "cocos2d.h"
#include "AppDelegate.h"
#include "TH_log.h"
#include "utility.h"

USING_NS_CC;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include <atlstr.h>
//#pragma comment(lib, "crashcatch_d10.lib")
//
#include <shellapi.h>
#include  <dbghelp.h> 
#pragma comment(lib,  "dbghelp.lib")


// ��ȡ��ϷĿ¼
CStringA makePath_ASCII( const char *filename )
{
	CStringA strPath;
	{
		char szAppPath[260];
		GetModuleFileNameA(GetModuleHandle(NULL), szAppPath, sizeof(szAppPath));
		int i;
		for(i=strlen(szAppPath)-1; i>0; i--) if(szAppPath[i]=='\\') break;
		szAppPath[i+1]=0;
		strPath = szAppPath;
	}
	if (filename)
	{
		strPath.Append(filename);
	}
	return strPath;
}

// �쳣����
LONG __stdcall unhandledExceptionFilterCallBack(PEXCEPTION_POINTERS pExceptionInfo)
{  
	//if (AppDelegate::isExitGame()) return EXCEPTION_EXECUTE_HANDLER;
	// д���쳣
	//filter_callback(pExceptionInfo);
	if (MessageBoxA(NULL, "�ǳ���Ǹ����Ϸ�����쳣�� ���ȷ����������Ϸ��", "��ʾ", MB_YESNO) == IDYES)
	{
		// HINSTANCE r = ShellExecuteA(NULL, "open", makePath_ASCII("clashofclans.exe"), NULL, NULL, SW_SHOWNORMAL);
		//AppDelegate::regameApp();
	}
	return EXCEPTION_EXECUTE_HANDLER;
}


LONG __stdcall UnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	CString   strDumpFile;
	CStringA strpath = makePath_ASCII(nullptr);
	strpath.Append("XXDMP.dmp");
	strDumpFile = strpath;
	HANDLE   hFile = CreateFile(strDumpFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION   ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = ExceptionInfo;
		ExInfo.ClientPointers = NULL;

		//   write   the   dump
		BOOL   bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
		CloseHandle(hFile);
	}

	if (MessageBoxA(NULL, "�ǳ���Ǹ����Ϸ�����쳣�� ���ȷ����������Ϸ��", "��ʾ", MB_YESNO) == IDYES)
	{
		 HINSTANCE r = ShellExecuteA(NULL, "open", makePath_ASCII("XXLZZ.exe"), NULL, NULL, SW_SHOWNORMAL);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

#endif

bool thCrashReport::initCrashReport( void )
{
	// ��ʼ��crash����
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CStringA strpath = makePath_ASCII(nullptr);

	// ע���쳣������
	SetUnhandledExceptionFilter(UnhandledExceptionFilter);

#endif

	return true;
}