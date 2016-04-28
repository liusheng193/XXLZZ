#include "TextReader.h"


//
// ���캯��
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

		if (*p == '\0')		// ���û�л�����
			break;

		if (*p == '\r')		// Windows����������"/r/n"
			*p++ = '\0';	// ���ַ�������Ҫ��'/r'��β
		if (*p == '\n')
			*p++ = '\0';
		lineno++;
	}
	data.push_back(ScriptData("end", 0));	// �������һ���յ�"end"
}
//
// ��������
//
TextReader::~TextReader()
{
	delete buffer;
}
