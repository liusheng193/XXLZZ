#ifndef	__TEXTREADER_H__
#define	__TEXTREADER_H__
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
//
// Script����
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


//����script������
class TextReader
{
public:
	TextReader(const char *name);
	~TextReader();
	//ȥ���Ѵ洢�����ĵ�����
	const char *GetFileName() const{ return filename.c_str(); };
	//����һ��
	const char* GetString(){ return data.size() > read_index ? data[read_index++].string : 0; };
	//ȡ����������е��к�
	int GetLineNo() const{ return data[read_index-1].lineno; }
	//�����Ƿ��Ѷ�ȡ���
	bool IsOk() const{ return buffer != 0; }
	operator bool() const{ return IsOk(); }

	//��������λ��
	int GetPosition() const{ return read_index; };
	//�趨����λ��
	void SetPosition(int position) { read_index = position; };
	//����һ������λ��
	void Back(){ read_index--; }
private:
	std::string filename;
	std::vector<ScriptData> data;
	char *buffer;
	int read_index;
};



#endif