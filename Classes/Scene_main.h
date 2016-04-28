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

// ������
class CScene_main : public cocos2d::Layer
{
	

	// ��Ŀ�����
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
	// �˵���Ŀ
	struct CMenuItem {
		char		text[MAX_MENU_TEXT];
		int			anser;					//�ڼ���
		int			length;					//����
		cocos2d::Color3B	color;					//��ɫ
	};
	// ����ϳ���Ŀ
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
	// ��ʼ��
	virtual bool init(void);
	// ����
	virtual void onEnter(void);
	// �뿪
	virtual void onExit(void);
	virtual void cleanup(void);

	//��Ϣ���ܴ���
	// ѡȡ/��ѡȡ��ʾ�˵���Ŀ
	void SelectMenu(int index, bool select);
	//ִ�е�һ������
	void OnFirstAction();
	//CLOSE�Ĵ���
	void OnClose();
	//��ȡ�ű�
	bool StartScript(const char *name, int mode = 0);
	//ִ�в˵�
	void StartMainMenu(void);
	// �л������뻭�����
	void SetGameLoadAction();
	// ���롤��ʾ����˵�
	void ShowLoadSaveMenu(bool isSave);


	void DeleteAllAction();
	bool RestartMusic();

	//���ز���
	CParams &GetParam(){ return Param; }
	// ս��������
	bool Battle(const char *name);
	// ��ȥ�ص�CG
	//
	// ʵ����ֻ�б����ʾ״��
	//
	bool ClearOverlap();
	// ��ȥ����CG
	bool ClearBack();
	//��ʾgb
	bool LoadImageBack(const char *name);
	//��ʾ�ص�cg
	bool LoadImageOverlap(const char *name);
	// ��ȡ��ҵ�õ�CG
	bool LoadImageWork(const char *name);
	//�˵�
	void ClearMenuItemCount() { MenuCount = 0; }
	int GetMenuItemCount() const { return MenuCount; }
	//���ò˵���Ŀ
	void SetMenuItem(const char *lavel, int anser);
	//���˵�
	void OpenMenu();
	//��ȡ�˵��Ľ����־
	int GetMenuAnser(int index) const { return MenuBuffer[index].anser; }
	//���ز˵�
	void HideMenuWindow(bool update = true);
	// ��ʾ�¼���Χ
	void ShowMessageWindow();
	// �����¼���Χ
	void HideMessageWindow(bool update = true);
	// ���¼���Χ��д����
	void WriteMessage(const char *msg);

	//���صȴ����
	void HideWaitMark();
	//������Ļ���װ�ã�Text Buffer��
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
	//������Ϣ
	void SentNotyMessage(UINT type);
	//��Ч���
public:
	void EffectRunAction(cocos2d::ActionInterval* act);
	//����
	void WipeIn();
	//����
	void WipeOut();
	//����
	void FadeIn();
	//����
	void FadeOut();
	//����
	void WipeIn(int nodename);
	void CutIn(int nodename);
	void MixFade(int nodename);
protected:

	//��Ч����
	void StopWipe();
	
	// �����Ļ���װ���и�ʽ���ַ������ٸ���
	int CFormatMessage(const char *msg);
	//����Ի��ı�
	void insertText(std::string msg);
protected:
	// �������result��
	typedef std::map<std::string, int, ics_less>	valuemap;
	valuemap	value_table;
public:
	int GetValue(std::string name) { return value_table[name]; }
	void SetValue(std::string name, int value) { value_table[name] = value; }
	void AddValue(std::string name, int value) { value_table[name] += value; }
	void DelValue(std::string name, int value) { value_table[name] -= value; }
	void ClearValue() { value_table.clear(); }
protected:
	//��ť����
	//��ʼ�˵���ť
	void _btn_Menu_CallBack(cocos2d::Ref*);


protected:
		// �������б仯ʱ�����»���
		bool Update(bool repaint=true);
		void HideAllLayer(cocos2d::Color3B pix);
		// װ����ʾ������
		void Mixing(int nodename , unsigned flags = ~0);
		//�Ƿ���õ�����
		bool Kinsoku(const char *p);
private:
	//��Ϣ����
	void _WindowProc(UINT uMsg, cocos2d::Vec2 pos = cocos2d::Vec2::ZERO);
	//ÿһ֡����
	void _OnUpdate(float dt);
protected:
	static CScene_main*		s_sceneMain;		// this
	CAction					*Action;				// ���ڵ�Action
	CParams					Param;		//����
	cocos2d::NodeGrid*		_gridNodeTarget;//��Ч
	cocos2d::NodeGrid*		_gridNodeTargetOV;//OV��Ч
	cocos2d::NodeGrid*		_gridNodeTargetBG;//BG��Ч
	CViewEffect				*ViewEffect;		// Ч��
	CMenuItem	MenuBuffer[MAX_MENU_ITEM];
	int			MenuCount;
	cocos2d::Sprite*		BackLayer;			// ����
	cocos2d::Sprite*		OverlapLayer;		//�ص�cg
	cocos2d::Node*			TextNode;			//�Ի���
	int			InvalidRect;		//��Ч��ģ������
	//int				MenuRect;			// �˵�ģ������

	std::string 	MsgBuffer[MessageLine];
	int		CurX;
	int		CurY;

	bool		TextDisplay;		// ��ʾ�������ݣ�
	bool		WaitMarkShowing;	// ��ʾ�ȴ��Ǻţ�

	bool		OverlapShow;		// ��ʾ�ص���
	bool		BackShow;			//��ʾ����
	bool		MenuShow;			// ��ʾ�˵���
	bool		SaveShow;			//��ʾ����/����
	bool		TextShow;			//����������ʾ
	bool		IsSaveMenu;
	bool		IsGridEffct;		//�Ƿ���grid��Ч
	struct	{
		bool		activate;
		char		title[MAX_SAVE_TEXT + 1];
		std::string	res;
	} DataTitle[PARAMS_MAX_SAVE];
};

//
// ��Чֹͣ
//
inline void CScene_main::StopWipe()
{
	//_gridNodeTarget->setGrid(nullptr);
	CC_SAFE_DELETE(ViewEffect);
	ViewEffect = nullptr;
}
#endif