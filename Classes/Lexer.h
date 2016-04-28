#ifndef _LEXER_H_
#define _LEXER_H_

#include "cocos2d.h"
//
// �����������Ľṹ��
//
struct LexValue	{
	std::string value;
	int type;
};


class Lexer
{
public:
	// �Ǻ�����
	enum
	{
		IsNumber,			//�Ƿ�����
		IsString,			//�Ƿ��ַ�
		IsDelimitter,		//�������
		IsLabel,			//�Ƿ��Ǳ�ǩ
		IsMinus,			//�Ƿ񸺺�
	};
private:
	enum
	{
		IsSpace = IsMinus + 1, //�Ƿ�ո�
		IsTerminater,		//�Ƿ��ս����
		IsQuotation,		//�Ƿ�Ӧ�á���
	};
public:
	Lexer(const char *str);
	//����token��Ŀ
	int NumToken() const;
	//��һ��token
	void NextToken(){ Count++; };
	//��ȡ�ƶ���token
	const char* GetString(int index=-1);
	bool GetValue(int *value, int index = -1);
	int GetType(int index = -1);
protected:
	//�����ո��ַ�
	const char* SkipSpace(const char *p);
	//�ַ�������ж�
	int CharType(unsigned char ch);
	//�ж��ַ������Ƿ�һ��
	bool MatchType(int &type, unsigned char ch);
protected:
	int nToken;				//һ���ڵ�token��Ŀ
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