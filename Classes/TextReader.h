#ifndef	__TEXTREADER_H__
#define	__TEXTREADER_H__
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
//
// Script资料
//
class ScriptData {
public:
	ScriptData() : status(0), lineno(0), string(0) {}
	ScriptData(const char *ptr, int no) : status(0), lineno((unsigned short)no), string(ptr) {}

public:
	unsigned char status;
	unsigned short lineno;
	const char *string;
};


//读入script档的类
class TextReader
{
public:
	TextReader(const char *name);
	~TextReader();
	//去的已存储档案的档案名
	const char *GetFileName() const{ return filename.c_str(); };
	//读入一行
	const char* GetString(){ return data.size() > read_index ? data[read_index++].string : 0; };
	//取得最后读入该行的行号
	int GetLineNo() const{ return data[read_index-1].lineno; }
	//档案是否已读取完毕
	bool IsOk() const{ return buffer != 0; }
	operator bool() const{ return IsOk(); }

	//读入资料位置
	int GetPosition() const{ return read_index; };
	//设定资料位置
	void SetPosition(int position) { read_index = position; };
	//传回一个读出位置
	void Back(){ read_index--; }
private:
	std::string filename;
	std::vector<ScriptData> data;
	char *buffer;
	int read_index;
};



#endif