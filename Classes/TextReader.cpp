#include "TextReader.h"


//
// 构造函数
//
TextReader::TextReader(const char *name)
:filename(name), read_index(0), buffer(0)
{
	auto log = FileUtils::getInstance()->getStringFromFile(name);

	std::string logstr(log.begin(), log.end());
	int length = logstr.length();
	buffer = new char[length + 1];
	snprintf(buffer, length + 1, "%s", logstr.c_str());
	char *endptr = buffer + length;
	char *p = buffer;
	int lineno = 1;
	while (p < endptr && *p != '\027') {
		data.push_back(ScriptData(p, lineno));

		while ((*p != '\r'&&*p != '\n')||(*p == ' '))
			p++;

		if (*p == '\0')		// 最后没有换行码
			break;

		if (*p == '\r')		// Windows的跳行码是"/r/n"
			*p++ = '\0';	// 故字符串后面要以'/r'结尾
		if (*p == '\n')
			*p++ = '\0';
		lineno++;
	}
	data.push_back(ScriptData("end", 0));	// 最后面多加一个空的"end"
}
//
// 析构函数
//
TextReader::~TextReader()
{
	delete buffer;
}
