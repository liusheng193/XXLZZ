#include "Params.h"
#include "ParamFile.h"
#include <stdio.h>
#include "File.h"
USING_NS_CC;



//有魔法吗？


#define	HEADER_ID	"DATA"


//
// 载入数据
//
static bool LoadData(const char *path, void *buf, int length)
{
	
	
	CFile	file(path, CFile::read);
	if (!file.IsOk())
	{
		//file.Close();
		return false;
	}
		

	if (file.Read(buf, length) != length)
	{
		//file.Close();
		return false;
	}
	
	file.Close();
	
	return true;
}
//
// 读出参数值
//
bool CParams::Load(int no)
{
	char	files[16];
	sprintf(files, "SAVE%04d.DAT", no + 1);
	auto file = FileUtils::getInstance()->fullPathForFilename(files);
	//if (file.length() <= 0)return false;
	unsigned char	buf[sizeof(CParams)+4];

	if (!LoadData(file.c_str(), buf, sizeof(buf)))
		return false;

	if (memcmp(buf, HEADER_ID, 4) != 0)
		return false;

	memcpy(this, buf + 4, sizeof(CParams));
	return true;
}