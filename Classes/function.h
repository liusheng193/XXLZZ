#ifndef __SCENE_MANAGE_H__
#define __SCENE_MANAGE_H__
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "iconv.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iconv.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "..\cocos2d\external\win32-specific\icon\include\iconv.h"
#endif

#pragma comment(lib,"libiconv.lib")
#include "string"
using namespace std;

//#ifndef A2U
//#define A2U

static int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	memset(outbuf, 0, outlen);
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (iconv(cd, (char **)pin, &inlen, pout, &outlen) == -1) return -1;
#else
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
#endif
	iconv_close(cd);
	return 0;
}

/*UTF8??GB2312*/
static std::string u2a(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	std::string strRet;
	if (code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete[] outbuf;
	return strRet;
}

/*GB2312??UTF8*/
static std::string a2u(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	std::string strRet;
	if (code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete[] outbuf;
	return strRet;
}

#endif