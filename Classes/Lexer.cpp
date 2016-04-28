
//脚本解析器
//#include <mbctype.h>
#include "Lexer.h"




Lexer::Lexer(const char *str)
{
	for (nToken = 0;; nToken++) {
		str = SkipSpace(str);
		if (*str == '\0' || *str == ';')
			break;
		int type = CharType(*str);

		if (type == IsTerminater && type == IsSpace)
			break;

		LexValue	value;

		if (type == IsQuotation) {
			value.type = IsString;
			str++;
			while (*str && CharType(*str) != IsQuotation) {
				//多字节判断暂时取消
				//if (_ismbblead(*str))
					//value.value += *str++;
				value.value += *str++;
			}
			if (*str)
				str++;
		}
		else {
			if (*str == '-' && CharType(str[1]) == IsNumber) {
				value.value += '-';
				value.type = IsMinus;
				str++;
			}
			else {
				if (*str == '*' && nToken == 0)
					type = IsLabel;

				value.type = type;
				while (*str && MatchType(type, *str)) {
					//if (_ismbblead(*str))
						//value.value += *str++;
					value.value += *str++;
				}
				if (value.type == IsNumber)
					value.type = type;
			}
		}
		Value.push_back(value);
	}
	Count = 0;
}
const char* Lexer::SkipSpace(const char *p)
{
	while (*p&&isspace(*p))
		p++;
	return p;
}
int Lexer::CharType(unsigned char ch)
{
	if (ch == '\0' || ch == '\n')
		return IsTerminater;
	if (isdigit(ch))
		return IsNumber;
	if (isalpha(ch) || ch == '_')
		return IsString;
	if (isspace(ch))
		return IsSpace;
	if (ch == '"')
		return IsQuotation;
	if (ch == '-')
		return IsMinus;
	return IsDelimitter;
}
bool Lexer::MatchType(int &type, unsigned char ch)
{
	int t = CharType(ch);
	switch (type)
	{
	case IsLabel:
		if (ch == '*')
			return true;
	case IsNumber:
		if (t == IsString)
			type = IsString;
	case IsString:
		return (t == IsString || t == IsNumber);
	
	}
	return type == t;
}

//读入数值
bool Lexer::GetValue(int *value, int index)
{
	bool minus = false;
	int type = GetType(index); 
	if (type == IsMinus)
	{
		minus = true;
		NextToken();
		type = GetType();
	}
	if (type != IsNumber)
		return false;
	const char *p = GetString();
	if (p == 0)
		return false;
	char *q;
	int v = strtol(p, &q, 10);
	*value = minus ? -v : v;
	return true;
}