#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "config.h"
#include "ui/CocosGUI.h"

typedef std::map<int, std::string> MapString;
typedef std::function<void(int, int)> funcMessageBoxCallBack;

class thUtility
{
public:
	static thUtility* getInstance();
	static void destroyInstance();
public:
	//�����ַ�������
	void loadStringFile(const std::string& file);
	//����ַ���
	static void pushString(int id,const std::string& str);
	static void popString(int id);
	//��ȡ�ַ���
	static std::string getString(int id);

	//����messagebox idΪ��Ϣ����,vBtnid������Ϣ�ڰ�ťid��
	static void addMessageBox(int id,cocos2d::Node* nd,VectorInt& vBtnid);
	static void removeMessageBox(int id);
	static void showMessageBox(int id,bool bShow);
	static void showMessageBox(int id,bool bShow,const std::string& str);
	//��Ϣ��ʾ�ص�
	static void addMessageBoxCallBack(int id,const funcMessageBoxCallBack& callback,int btnid,bool bPop);

	//���ñ��� cocosע������ΪTitletext�Ĳ��Ǳ���
	static void setMessageBoxTitle(const std::string& str,int id=-1);
	//��������contexttext
	static void setMessageBoxString(const std::string& str,int id=-1);

	//��ȡʱ��
	static void analyTime(int time,sTime &out);

	// ��ʽ��ʱ��ת��Ϊ����
	static void formatTime(int time, char *szbuf,int size);
	//sudo Ϊʲô������
	//static void formatTime2(int time,char *szbuf,int size);

	//�ַ�����ȡ
	static std::string analyString(std::string &str,char tp);
	//����int
	static int analyInt(std::string &str,char type);
	static float analyFloat(std::string &str,char type);
	//�ַ���ȡ
	static void analyStrToStr(std::string &str,char tp,const std::string &to);
	static int GBKToUTF8(std::string &gbkStr);
public:
	//��ȡ�ļ���
	static std::string getFileName(const std::string& str);
public :
	// ���ð�ť�Ƿ�����
	static void setButtonEnable(cocos2d::Node* btn,bool enable);
	//��ť����Ч������
	static void beginButtonScaleEffect(cocos2d::Node* btn);
	static void endButtonScaleEffect(cocos2d::Node* btn);

protected:
	//��Ϣ�ص�
	void messageBoxCallBack(cocos2d::Ref* ref,cocos2d::ui::Widget::TouchEventType type);
protected:
	thUtility();
	~thUtility();
protected:
	MapString m_mapString;

	struct sMessageBoxCall
	{
		funcMessageBoxCallBack callfunc;
		int btnID;
		bool bPop;
	};
	typedef std::vector<sMessageBoxCall> FuncCallBack;
	struct sMessageBox
	{
		cocos2d::Node* messagebox;
		FuncCallBack vFuncCallBack;
	};
	typedef std::map<int, sMessageBox> MapMessageBox;
	MapMessageBox m_mapMessageBox;
	int	m_curMessageBoxId;
};

#endif