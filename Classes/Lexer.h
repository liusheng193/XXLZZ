#ifndef _LEXER_H_
#define _LEXER_H_

#include "cocos2d.h"
//
// 储存分析结果的结构体
//
struct LexValue	{
	std::string value;
	int type;
};


class Lexer
{
public:
	// 记号种类
	enum
	{
		IsNumber,			//是否数字
		IsString,			//是否字符
		IsDelimitter,		//定界符号
		IsLabel,			//是否是标签
		IsMinus,			//是否负号
	};
private:
	enum
	{
		IsSpace = IsMinus + 1, //是否空格
		IsTerminater,		//是否终结符号
		IsQuotation,		//是否应用“”
	};
public:
	Lexer(const char *str);
	//返回token数目
	int NumToken() const;
	//下一个token
	void NextToken(){ Count++; };
	//读取制定的token
	const char* GetString(int index=-1);
	bool GetValue(int *value, int index = -1);
	int GetType(int index = -1);
protected:
	//跳过空格字符
	const char* SkipSpace(const char *p);
	//字符种类的判断
	int CharType(unsigned char ch);
	//判断字符种类是否一致
	bool MatchType(int &type, unsigned char ch);
protected:
	int nToken;				//一行内的token数目
	std::vector<LexValue> Value;
	int					  Count;

};
inline int Lexer::NumToken() const
{
	return nToken;
}
inline const char* Lexer::GetString(int index)
{
	if (index >= 0)
		Count = index;
	if (Count >= nToken)
		return NULL;
	return Value[Count++].value.c_str();

}
inline int Lexer::GetType(int index)
{
	if (index > 0)
		Count = index;
	if (Count >= nToken)
		return -1;
	return Value[Count].type;
}
#endif