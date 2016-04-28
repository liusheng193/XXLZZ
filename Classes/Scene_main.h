#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Config.h"
#include "Action.h"
#include "Params.h"
#include "Effect.h"
#include "script.h"

#define BUF_SIZE_MAIN		1024

// 主场景
class CScene_main : public cocos2d::Layer
{
	

	// 项目最大数
	enum	{
		MAX_MENU_ITEM = 8,
		MAX_MENU_TEXT = 60,
		MAX_SAVE_TEXT = 62,
	};
	enum {
		AllRect=9,
		TextRect,
		MenuRect,
		SaveRect,
		WaitMarkRect,
	};
	// 菜单项目
	struct CMenuItem {
		char		text[MAX_MENU_TEXT];
		int			anser;					//第几行
		int			length;					//长度
		cocos2d::Color3B	color;					//颜色
	};
	// 画面合成项目
	enum	{
		TextMessage = 1 << 0,
		TextWaitMark = 1 << 1,
		MenuFrame = 1 << 2,
		SaveTitle = 1 << 3,
		MenuItemFirst = 4,
		SaveItemFirst = 12,
	};

public:
	CScene_main(void);
	~CScene_main(void);
	CREATE_FUNC(CScene_main);
	static CScene_main* getInstance(void);
	static cocos2d::Scene* createScene();
	// 初始化
	virtual bool init(void);
	// 进入
	virtual void onEnter(void);
	// 离开
	virtual void onExit(void);
	virtual void cleanup(void);

	//消息接受处理
	// 选取/非选取显示菜单项目
	void SelectMenu(int index, bool select);
	//执行第一步操作
	void OnFirstAction();
	//CLOSE的处理
	void OnClose();
	//读取脚本
	bool StartScript(const char *name, int mode = 0);
	//执行菜单
	void StartMainMenu(void);
	// 切换到载入画面操作
	void SetGameLoadAction();
	// 载入·显示储存菜单
	void ShowLoadSaveMenu(bool isSave);


	void DeleteAllAction();
	bool RestartMusic();

	//返回参数
	CParams &GetParam(){ return Param; }
	// 战斗画面用
	bool Battle(const char *name);
	// 消去重叠CG
	//
	// 实际上只有变更显示状况
	//
	bool ClearOverlap();
	// 消去背景CG
	bool ClearBack();
	//显示gb
	bool LoadImageBack(const char *name);
	//显示重叠cg
	bool LoadImageOverlap(const char *name);
	// 读取作业用的CG
	bool LoadImageWork(const char *name);
	//菜单
	void ClearMenuItemCount() { MenuCount = 0; }
	int GetMenuItemCount() const { return MenuCount; }
	//设置菜单项目
	void SetMenuItem(const char *lavel, int anser);
	//描绘菜单
	void OpenMenu();
	//获取菜单的结果标志
	int GetMenuAnser(int index) const { return MenuBuffer[index].anser; }
	//隐藏菜单
	void HideMenuWindow(bool update = true);
	// 显示事件范围
	void ShowMessageWindow();
	// 隐藏事件范围
	void HideMessageWindow(bool update = true);
	// 在事件范围中写内容
	void WriteMessage(const char *msg);

	//隐藏等待标记
	void HideWaitMark();
	//清除内文缓冲装置（Text Buffer）
	void ClearMessage();

	const int &GetInvalidRect() const { return InvalidRect; }
	void Invalidate(int type);// { InvalidRect == str; }
	void Invalidate();
	// msg
public:
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	virtual void onEnterTransitionDidFinish(void);
	//发送消息
	void SentNotyMessage(UINT type);
	//特效相关
public:
	void EffectRunAction(cocos2d::ActionInterval* act);
	//擦入
	void WipeIn();
	//擦除
	void WipeOut();
	//淡入
	void FadeIn();
	//淡出
	void FadeOut();
	//擦除
	void WipeIn(int nodename);
	void CutIn(int nodename);
	void MixFade(int nodename);
protected:

	//特效结束
	void StopWipe();
	
	// 在内文缓冲装置中格式化字符串后再复制
	int CFormatMessage(const char *msg);
	//插入对话文本
	void insertText(std::string msg);
protected:
	// 变量表格result等
	typedef std::map<std::string, int, ics_less>	valuemap;
	valuemap	value_table;
public:
	int GetValue(std::string name) { return value_table[name]; }
	void SetValue(std::string name, int value) { value_table[name] = value; }
	void AddValue(std::string name, int value) { value_table[name] += value; }
	void DelValue(std::string name, int value) { value_table[name] -= value; }
	void ClearValue() { value_table.clear(); }
protected:
	//按钮处理
	//开始菜单按钮
	void _btn_Menu_CallBack(cocos2d::Ref*);


protected:
		// 若画面有变化时则重新绘制
		bool Update(bool repaint=true);
		void HideAllLayer(cocos2d::Color3B pix);
		// 装配显示的内容
		void Mixing(int nodename , unsigned flags = ~0);
		//是否禁用的文字
		bool Kinsoku(const char *p);
private:
	//消息处理
	void _WindowProc(UINT uMsg, cocos2d::Vec2 pos = cocos2d::Vec2::ZERO);
	//每一帧处理
	void _OnUpdate(float dt);
protected:
	static CScene_main*		s_sceneMain;		// this
	CAction					*Action;				// 现在的Action
	CParams					Param;		//参数
	cocos2d::NodeGrid*		_gridNodeTarget;//特效
	cocos2d::NodeGrid*		_gridNodeTargetOV;//OV特效
	cocos2d::NodeGrid*		_gridNodeTargetBG;//BG特效
	CViewEffect				*ViewEffect;		// 效果
	CMenuItem	MenuBuffer[MAX_MENU_ITEM];
	int			MenuCount;
	cocos2d::Sprite*		BackLayer;			// 背景
	cocos2d::Sprite*		OverlapLayer;		//重叠cg
	cocos2d::Node*			TextNode;			//对话框
	int			InvalidRect;		//无效化模块名称
	//int				MenuRect;			// 菜单模块名称

	std::string 	MsgBuffer[MessageLine];
	int		CurX;
	int		CurY;

	bool		TextDisplay;		// 显示文字内容？
	bool		WaitMarkShowing;	// 显示等待记号？

	bool		OverlapShow;		// 显示重叠？
	bool		BackShow;			//显示背景
	bool		MenuShow;			// 显示菜单？
	bool		SaveShow;			//显示储存/载入
	bool		TextShow;			//文字区域显示
	bool		IsSaveMenu;
	bool		IsGridEffct;		//是否是grid特效
	struct	{
		bool		activate;
		char		title[MAX_SAVE_TEXT + 1];
		std::string	res;
	} DataTitle[PARAMS_MAX_SAVE];
};

//
// 特效停止
//
inline void CScene_main::StopWipe()
{
	//_gridNodeTarget->setGrid(nullptr);
	CC_SAFE_DELETE(ViewEffect);
	ViewEffect = nullptr;
}
#endif