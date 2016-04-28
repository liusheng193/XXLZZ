#ifndef	__CFILE_H__
#define	__CFILE_H__
#include<iostream>

class CFile
{
public:
	enum 
	{
		read
	};
public:
	CFile(const char *file,int mode);
	~CFile();
	bool Open(const char *file, int mode);
	bool Close();
	int Read(void*buffer, int length);
	int Write(const void *buffer, int length);
	long Seek(long offset, int from);
	int GetFileSize();
	bool IsOk();
	bool Error();
private:
	FILE *fp;
};
inline CFile::CFile(const char *file, int mode) :fp(nullptr)
{
	Open(file, mode);
}
inline CFile::~CFile()
{
	Close();
}
inline bool CFile::Open(const char *file, int mode)
{
	fp = fopen(file, mode == CFile::read ? "rb":"wb");
	return fp != nullptr;
}
inline bool CFile::Close()
{
	if (fp!=nullptr)
	{
		fclose(fp);
		fp = nullptr;
	}
	return true;
}
inline int CFile::Read(void*buffer, int length)
{
	return fread(buffer, 1, length, fp);
}
inline int CFile::Write(const void *buffer, int length)
{
	return fwrite(buffer, 1, length, fp);
}
inline long  CFile::Seek(long offset, int from)
{
	return fseek(fp, offset, from);
}
inline int CFile::GetFileSize()
{
	return 0;
	//int handle = _fileleng _fileno(fp);
}
inline bool CFile::IsOk()
{
	if (fp)return true;
	return false;
}
inline bool CFile::Error()
{
	if (ferror(fp) > 0)return true;
	else
		return false;
}
#endif