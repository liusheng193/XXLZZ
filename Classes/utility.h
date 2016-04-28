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
	//加载字符串配置
	void loadStringFile(const std::string& file);
	//添加字符串
	static void pushString(int id,const std::string& str);
	static void popString(int id);
	//获取字符串
	static std::string getString(int id);

	//设置messagebox id为消息类型,vBtnid保存消息内按钮id等
	static void addMessageBox(int id,cocos2d::Node* nd,VectorInt& vBtnid);
	static void removeMessageBox(int id);
	static void showMessageBox(int id,bool bShow);
	static void showMessageBox(int id,bool bShow,const std::string& str);
	//消息提示回调
	static void addMessageBoxCallBack(int id,const funcMessageBoxCallBack& callback,int btnid,bool bPop);

	//设置标题 cocos注意名字为Titletext的才是标题
	static void setMessageBoxTitle(const std::string& str,int id=-1);
	//设置内容contexttext
	static void setMessageBoxString(const std::string& str,int id=-1);

	//获取时间
	static void analyTime(int time,sTime &out);

	// 格式化时间转换为中文
	static void formatTime(int time, char *szbuf,int size);
	//sudo 为什么有两个
	//static void formatTime2(int time,char *szbuf,int size);

	//字符串截取
	static std::string analyString(std::string &str,char tp);
	//解析int
	static int analyInt(std::string &str,char type);
	static float analyFloat(std::string &str,char type);
	//字符截取
	static void analyStrToStr(std::string &str,char tp,const std::string &to);
	static int GBKToUTF8(std::string &gbkStr);
public:
	//获取文件名
	static std::string getFileName(const std::string& str);
public :
	// 设置按钮是否启用
	static void setButtonEnable(cocos2d::Node* btn,bool enable);
	//按钮缩放效果处理
	static void beginButtonScaleEffect(cocos2d::Node* btn);
	static void endButtonScaleEffect(cocos2d::Node* btn);

protected:
	//消息回调
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