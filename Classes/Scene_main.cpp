#include "Scene_main.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Script.h"
#include "cd_visibleRect.h"
#include "utility.h"
//#include "function.h"

CScene_main* CScene_main::s_sceneMain = nullptr;

USING_NS_CC;
using namespace cocostudio;
using namespace CocosDenshion;

#define WINDPOST  "WindowProc"

#define TAG_MAIN_UI_PROP_T	203
#define TAG_MAIN_UI_MASK_T	202

#define TAG_MAIN_UI_PROP	201
#define TAG_MAIN_UI_MASK	200
#define TAG_MAIN_UI			199
#define TAG_MAIN_UI_P		198
#define TAG_MAIN_OV			197
#define TAG_MAIN_BG			196
#define TAG_MAIN_UI_PROP_BUY	6001
#define TAG_MAIN_UI_MASK_BUY	6000

#define TAG_MAIN_UI_INFO	2001
#define TAG_MAIN_UI_MASK1	2000

#define TAG_MAIN_UI_LOAD	12000


CScene_main* CScene_main::getInstance(void)
{
	return s_sceneMain;
}

CScene_main::CScene_main(void)
:_gridNodeTarget(nullptr),
TextNode(nullptr),
ViewEffect(nullptr),
OverlapLayer(nullptr),
BackLayer(nullptr),
MenuShow(false),
SaveShow(false),
TextDisplay(false),
TextShow(false),
WaitMarkShowing(false),
InvalidRect(0),
IsGridEffct(false),
_gridNodeTargetOV(nullptr),
_gridNodeTargetBG(nullptr)
{
	s_sceneMain = this;
}

CScene_main::~CScene_main(void)
{
	DeleteAllAction();
	Director::getInstance()->getScheduler()->unscheduleAll();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	s_sceneMain = nullptr;
	ViewEffect = nullptr;
	this->stopAllActions();
	this->removeAllChildren();
}

Scene* CScene_main::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = CScene_main::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool CScene_main::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Action = new CAction(this);	// 虚拟动作（dummy action）

	//MenuRect = 0;
	//接受消息
	//NotificationCenter::getInstance()->addObserver(this,)
	//__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(CScene_main::_WindowProc), WINDPOST, nullptr);
	//发送消息
	//auto node = Node::create();
	//node->setUserData("usr");
	//__NotificationCenter::getInstance()->postNotification(WINDPOST, (Ref*)node);
	//this->addChild(node);
	//定时处理
	this->schedule(schedule_selector(CScene_main::_OnUpdate), 0.0f);


	_gridNodeTarget = NodeGrid::create();
	_gridNodeTarget->setCascadeOpacityEnabled(true);
	addChild(_gridNodeTarget, 0);
	
	//BG特效
	_gridNodeTargetBG = NodeGrid::create();
	_gridNodeTargetBG->setCascadeOpacityEnabled(true);
	_gridNodeTarget->addChild(_gridNodeTargetBG, TAG_MAIN_BG);
	//OV特效
	_gridNodeTargetOV = NodeGrid::create();
	_gridNodeTargetOV->setCascadeOpacityEnabled(true);
	_gridNodeTarget->addChild(_gridNodeTargetOV, TAG_MAIN_OV);

	//bg显示
	BackLayer = Sprite::create();
	//_gridNodeTargetBG->setTarget(BackLayer);
	_gridNodeTargetBG->addChild(BackLayer, TAG_MAIN_BG);
	_gridNodeTarget->setAnchorPoint(Vec2(0.5f, 0.5f));

	//人物cg
	OverlapLayer = Sprite::create();
	//_gridNodeTargetOV->setTarget(OverlapLayer);
	_gridNodeTargetOV->addChild(OverlapLayer, TAG_MAIN_OV);


	//对话框
	auto textspr = Sprite::create();
	TextNode = CSLoader::createNode(UIPATH"UI_Talk.csb");
	TextNode->setTag(1);
	textspr->addChild(TextNode);
	//_gridNodeTarget->addChild(TextNode, TAG_MAIN_UI_P);
	_gridNodeTarget->addChild(textspr, TAG_MAIN_UI_P);
	//auto listview = dynamic_cast<ui::ListView*>(TextNode->getChildByTag(3));
	//listview->setCascadeOpacityEnabled(true);
	textspr->setTag(TextRect);
	textspr->setVisible(false);
	//适配
	auto scenesize = CVisibleRect::getVisibleRect().size;
	auto bgsize = Size(WindowWidth, WindowHeight);
	ResolutionPolicy rp = ResolutionPolicy::FIXED_HEIGHT;
	auto resolutionBg = CVisibleRect::getDesignResolutionInfo(rp, bgsize);
	_gridNodeTarget->setScale(resolutionBg.fBGScaleX, resolutionBg.fBGScaleY);
	auto pos = CVisibleRect::center();
	//BackLayer->setScale(resolutionBg.fBGScaleX, resolutionBg.fBGScaleY);
	BackLayer->setPosition(pos);
	//BackLayer->setVisible(false);
	//OverlapLayer->setScale(resolutionBg.fBGScaleX, resolutionBg.fBGScaleY);
	OverlapLayer->setPosition(pos);
	OverlapLayer->setVisible(false);
	
	textspr->setPosition(CVisibleRect::bottom());
	//发送第一个命令
	_WindowProc(PosNType::PS_FIRSTACTION);


	return true;
}

void CScene_main::_OnUpdate(float dt)
{
	if (ViewEffect)
	{
		if (ViewEffect->Step(0))	// 特效执行
			return ;
		StopWipe();								// 特效结束
		if (IsGridEffct)
		{
			_gridNodeTarget->setGrid(nullptr);
			_gridNodeTargetBG->setGrid(nullptr);
			_gridNodeTargetOV->setGrid(nullptr);
			IsGridEffct = false;
		}
		Action->WipeDone();						// 结束通报给动作
	}
	Action->IdleAction();
}
void CScene_main::_WindowProc(UINT uMsg, cocos2d::Vec2 pos)
{
	switch (uMsg) {
	case PosNType::PS_FIRSTACTION:		// 最初传送的事件
		OnFirstAction();
		break;

	case PosNType::PS_CLOSE:			// 关闭窗口
		OnClose();
		break;

	case PosNType::PS_ERASEBKGND:		// 删除背景
		return ;			// 无动作

	case PosNType::PS_LBUTTONDOWN:		// 按下鼠标左键
		Action->LButtonDown(uMsg, pos);
		break;

	case PosNType::PS_LBUTTONUP:		// 放开鼠标左键
		Action->LButtonUp(uMsg, pos);
		break;

	case PosNType::PS_RBUTTONDOWN:		// 按下鼠标右键
		//Action->RButtonDown(wParam, CPoint(lParam));
		break;

	case PosNType::PS_RBUTTONUP:		// 放开鼠标右键
		//Action->RButtonUp(wParam, CPoint(lParam));
		break;

	case PosNType::PS_MOUSEMOVE:		// 鼠标指标移动
		//Action->MouseMove(wParam, CPoint(lParam));
		break;

	case PosNType::PS_KEYDOWN:			// 按下键盘
		//Action->KeyDown(wParam);
		break;

	case PosNType::PS_DESTROY:			// 取消窗口
		//OnDestroy();
		break;

	case PosNType::PS_TIMER:			// 计时器到期
		//OnTimer(wParam);
		break;

	case PosNType::PS_MCINOTIFY:		// MCI的事件
		//OnMciNotify(wParam, LOWORD(lParam));
		break;

	default:
		return;// CWindow::WindowProc(uMsg, wParam, lParam);
	}
	return ;
	//auto node = (Node*)sender;
	//char* buf = static_cast<char*>(node->getUserData());
	//log("%s", buf);
	//OnFirstAction();
}
void CScene_main::OnFirstAction()
{
	//开始音乐todo
	//读取脚本
	StartMainMenu();
}
void CScene_main::StartMainMenu()
{
	DeleteAllAction();
	if (!StartScript("main",CScriptAction::MODE_SYSTEM))
		Director::getInstance()->end();
}
bool CScene_main::StartScript(const char *name, int mode)
{
	CScriptAction *action = new CScriptAction(this, Action, mode);
	if (!action->Load(name)) {			// 读入脚本
		delete action;
		return false;
	}

	Action = action;
	//PostMessage(WM_KICKIDLE);	// 为了慎重起见发送空事件
	return true;
}
void CScene_main::onEnter(void)
{
	Layer::onEnter();
}

void CScene_main::onExit(void)
{
	Layer::onExit();
}

void CScene_main::cleanup(void)
{
	Layer::cleanup();
}

bool CScene_main::RestartMusic()
{
	return false;
}
void CScene_main::SelectMenu(int index, bool select)
{
	if (index >= 0)
	{

	}
}


//
// 删除动作
//
void CScene_main::DeleteAllAction()
{
	//__NotificationCenter::getInstance()->removeAllObservers(this);
	while (Action) {
		CAction *action = Action->GetOldAction();
		delete Action;
		Action = action;
	}
}
void CScene_main::Invalidate()
{ 
	//BackLayer = nullptr;
	//b
	InvalidRect = AllRect;
}
void CScene_main::Invalidate(int type)
{ 
	InvalidRect = type;
}
bool CScene_main::ClearOverlap()
{
	OverlapShow = false;
	Invalidate();
	return true;
}
bool CScene_main::ClearBack()
{
	BackShow = false;
	Invalidate();
	return true;
}
bool CScene_main::LoadImageBack(const char *name)
{
	BackShow = true;
	//Invalidate();
	char bzbuf[256];
	sprintf(bzbuf, "%s.png", name);
	BackLayer->initWithSpriteFrameName(bzbuf);
	//BackLayer->setTexture(bzbuf);//= Sprite::create("HelloWorld.png");
	return true;
}
// 读取作业用的CG
//
bool CScene_main::LoadImageOverlap(const char *name)
{
	OverlapShow = true;
	char bzbuf[256];
	sprintf(bzbuf, "%s.png", name);
	OverlapLayer->initWithSpriteFrameName(bzbuf);
	return true;
}
bool CScene_main::LoadImageWork(const char *name)
{
	OverlapShow = false;
	char bzbuf[256];
	sprintf(bzbuf, "%s.png", name);
	return OverlapLayer->initWithSpriteFrameName(bzbuf);
}
#define	STR_LIMIT	(MessageWidth - 2)
#define	STR_WIDTH	(STR_LIMIT - 2)

//
// 清除内文缓冲装置（Text Buffer）
//
void CScene_main::ClearMessage()
{
	HideMessageWindow();

	CurX = CurY = 0;

	for (int i = 0; i<MessageLine; i++)
		MsgBuffer[i]= "";
}
//
// 在内文缓冲装置中格式化字符串后再复制
//
// 因为正在处理禁用条例、限定日文
//
int CScene_main::CFormatMessage(const char *msg)
{
	CurX = CurY = 0;
	std::string str = msg;
	for (int i = 0; i<MessageLine; i++)
		MsgBuffer[i] = "";
	int idot = str.rfind('\n');
	int start = 0;
	while (idot>0)
	{
		MsgBuffer[CurY++] = str.substr(start, idot + 1);
		start = idot + 1;
		str = str.substr(start, str.length());
		idot = str.rfind('\n');
	}
	//while (*msg && CurY < MessageLine) {
	//	if (*msg == '\n') {
	//		msg++;
	//		//CurX = 0;
	//		CurY++;
	//	}
	//	else if (CurX >= STR_WIDTH)
	//	{
	//		MsgBuffer[CurY][CurX] = 0;
	//		CurX = 0;
	//		CurY++;
	//	}
	//	else  {
	//		if (CurX >= STR_LIMIT || (CurX >= STR_WIDTH && Kinsoku(msg) == 0)) {
	//			MsgBuffer[CurY][CurX] = 0;
	//			CurX = 0;
	//			CurY++;
	//		}
	//		MsgBuffer[CurY][CurX++] = *msg++;
	//		MsgBuffer[CurY][CurX++] = *msg++;
	//	}
	//	//多字节判断暂时取消
	//	/*else if (_ismbblead(*msg)) {
	//		if (CurX >= STR_LIMIT || (CurX >= STR_WIDTH && Kinsoku(msg) == 0)) {
	//			MsgBuffer[CurY][CurX] = 0;
	//			CurX = 0;
	//			CurY++;
	//		}
	//		MsgBuffer[CurY][CurX++] = *msg++;
	//		MsgBuffer[CurY][CurX++] = *msg++;
	//	}
	//	else {
	//		if (CurX >= STR_WIDTH) {
	//			MsgBuffer[CurY][CurX] = 0;
	//			CurX = 0;
	//			CurY++;
	//		}*/
	//		MsgBuffer[CurY][CurX++] = *msg++;
	//	}
	//
	//if (CurX > 0 && CurY < MessageLine) {
	//	MsgBuffer[CurY][CurX] = 0;
	//	CurY++;
	//}
	return 0;//CurY;
}
bool CScene_main::Kinsoku(const char *p)
{
	switch (p[0]) {
	case 0x81:
		switch (p[1]) {
		case 0x41:
		case 0x42:
		case 0x49:
		case 0x48:
		case 0x5B:
		case 0x6A:
		case 0x76:
		case 0x78:
		case 0x99:
		case 0xf4:
			return true;
		}
		break;

	case 0x82:
		switch (p[1]) {
		case 0x9f:
		case 0xa1:
		case 0xa3:
		case 0xa5:
		case 0xa7:
		case 0xe1:
		case 0xe3:
		case 0xe5:
		case 0xc1:
			return true;
		}
		break;

	case 0x83:
		switch (p[1]) {
		case 0x40:
		case 0x42:
		case 0x44:
		case 0x46:
		case 0x48:
		case 0x83:
		case 0x85:
		case 0x87:
		case 0x62:
			return true;
		}
	}
	return false;
}
//
// 在事件范围中写内容
//
void CScene_main::WriteMessage(const char *msg)
{
	CFormatMessage(msg);

	WaitMarkShowing = true;

	ShowMessageWindow();
}
//
// 显示事件范围
//
void CScene_main::ShowMessageWindow()
{
	TextDisplay = true;

	TextShow = true;
	Invalidate(TextRect);
	Update(false);
}
void CScene_main::HideMessageWindow(bool update)
{
	TextDisplay = false;

	if (TextShow) {
		TextShow = false;
		Invalidate(TextRect);
		if (update)
			Update();
	}
}
void CScene_main::EffectRunAction(cocos2d::ActionInterval* act)
{
	_gridNodeTarget->runAction(act);
}

void CScene_main::WipeIn()
{
	IsGridEffct = true;
	ViewEffect = new EffectWipeIn(this,"",Rect());
}

void CScene_main::WipeOut()
{
	IsGridEffct = true;
	ViewEffect = new EffectWipeOut(this, "");
}
void CScene_main::FadeIn()
{
	IsGridEffct = false;
	Update(false);
	_gridNodeTarget->setOpacity(0);
	//OverlapLayer->setOpacity(75);
	ViewEffect = new EffectFadeIn(this, "");
}
void CScene_main::FadeOut()
{
	IsGridEffct = false;
	HideMessageWindow();
	ViewEffect = new EffectFadeOut(this, "");
	HideAllLayer(Color3B::BLACK);
}

void CScene_main::WipeIn(int nodename)
{
	Update(false);
}
void CScene_main::CutIn(int nodename)
{
	Update(false);
}
void CScene_main::MixFade(int nodename)
{
	Update(false);
}
// 画面的全部设定成不显示
//
// pix 是填满画面的颜色
//
void CScene_main::HideAllLayer(Color3B pix)
{
	//BgColor = pix;
	BackShow = false;
	OverlapShow = false;
}

//
// 设置菜单项目
//
void CScene_main::SetMenuItem(const char *str, int anser)
{
	int		n = strlen(str);
	memcpy(MenuBuffer[MenuCount].text, str, n + 1);
	MenuBuffer[MenuCount].anser = anser;
	MenuBuffer[MenuCount].length = n;
	MenuBuffer[MenuCount].color = Color3B::WHITE;
	MenuCount++;
}
void CScene_main::OpenMenu()
{
	MenuShow = true;
	auto s = Director::getInstance()->getWinSize();
	//MenuRect = MenuRect;
	auto node = Node::create();
	node->setTag(MenuRect);
	node->setAnchorPoint(Vec2(0.5f, 0.5f));
	node->setPosition(CVisibleRect::center());
	for (int i = 0; i < MenuCount; i++) {
		auto label = Label::createWithTTF(TTFConfig(FONTPATH,24),  a2u(MenuBuffer[i].text).c_str());
		label->setColor(MenuBuffer[i].color);
		auto mi = MenuItemLabel::create(label, CC_CALLBACK_1(CScene_main::_btn_Menu_CallBack, this));
		auto menu = Menu::create(mi, nullptr);
		//this->addChild(menu, TAG_MAIN_UI);
		//std::string str = ;
		//auto txti = ui::Text::create(str, FONTPATH, 16);
		//char szbuf[256];
		//snprintf(szbuf, 256, "%s", thUtility::getString(112).c_str());
		//txti->setString(szbuf);
		menu->setPosition( Vec2(0, -(label->getContentSize().height+6)*i ));
		mi->setTag(i);
		node->addChild(menu);
		
		
	}
	_gridNodeTarget->addChild(node, TAG_MAIN_UI);
	node->setVisible(false);
	Mixing(MenuRect);
	
}

void CScene_main::_btn_Menu_CallBack(cocos2d::Ref* ret)
{
	auto node = dynamic_cast<cocos2d::Node*>(ret);
	log("button:%d", node->getTag());
	SentNotyMessage(PosNType::PS_LBUTTONDOWN);
	Action->SetMenuSelect(node->getTag());
	SentNotyMessage(PosNType::PS_LBUTTONUP);
}
bool CScene_main::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	SentNotyMessage(PosNType::PS_LBUTTONDOWN);
	return true;
}
void CScene_main::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
}
void CScene_main::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
}
void CScene_main::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
}
void CScene_main::onEnterTransitionDidFinish(void)
{
	// 检测所有消息
	auto listenerOne = EventListenerTouchOneByOne::create();
	listenerOne->onTouchBegan = CC_CALLBACK_2(CScene_main::onTouchBegan, this);
	listenerOne->onTouchEnded = CC_CALLBACK_2(CScene_main::onTouchEnded, this);
	listenerOne->onTouchMoved = CC_CALLBACK_2(CScene_main::onTouchEnded, this);
	listenerOne->onTouchCancelled = CC_CALLBACK_2(CScene_main::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listenerOne, -1);
}

void CScene_main::HideMenuWindow(bool update)
{
	if (MenuShow) {
		MenuShow = false;
		Invalidate(MenuRect);
		if (update)
			Update();
	}
}
// 若画面有变化时则重新绘制
//
bool CScene_main::Update(bool repaint)
{
	if (InvalidRect > 0)
	{
		Mixing(InvalidRect);
		if (repaint)
		{
			if (InvalidRect == AllRect)
			{
				auto childnode = _gridNodeTarget->getChildren();
				int idx = 0;
				for (auto &v : childnode)
				{
					v->setVisible(false);
				}
			}
			//重新绘制
			else
			if (_gridNodeTarget->getChildByTag(InvalidRect))
				_gridNodeTarget->getChildByTag(InvalidRect)->setVisible(false);
			
		}
		InvalidRect = 0;
		return true;
	}
	return false;
}
void CScene_main::insertText(std::string msg)
{
	if (!TextNode)return;
	if (msg.length() <= 0)return;
	auto listview = dynamic_cast<ui::ListView*>(TextNode->getChildByTag(3));
	auto layout = ui::Layout::create();
	layout->setContentSize(Size(780, 20));
	auto nodezb = Node::create();
	layout->addChild(nodezb);
	listview->pushBackCustomItem(layout);
	auto txt = ui::Text::create(a2u(msg.c_str()), FONTPATH, 20);
	txt->setAnchorPoint(Vec2(0, 1));
	txt->setPosition(Vec2::ZERO);
	layout->addChild(txt);
	listview->refreshView();
	listview->jumpToBottom();

}
void CScene_main::Mixing(int nodename, unsigned flags)
{

	if (BackShow)
	{
		BackLayer->setVisible(true);		// 有背景的话就复制
	}
	else
		BackLayer->setVisible(false);	// 没有背景的话就涂满

	// 重叠
	if (OverlapShow)							// 有重叠的话就合成
	{
		
		OverlapLayer->setVisible(true);
	}
	else
		OverlapLayer->setVisible(false);
	// 储存与载入菜单
	if (SaveShow) {
	}
	else
	{
		if (TextShow) {
			if (flags & TextMessage) {
				//显示文本提示
				_gridNodeTarget->getChildByTag(nodename)->getChildByTag(1)->getChildByTag(4)->setVisible(false);
				log("%d",_gridNodeTarget->getChildByTag(nodename)->getOpacity());
				_gridNodeTarget->getChildByTag(nodename)->setVisible(true);
				log("%d", _gridNodeTarget->getChildByTag(nodename)->isVisible()?1:0);
				auto list = dynamic_cast<ui::ListView*> (_gridNodeTarget->getChildByTag(nodename)->getChildByTag(1)->getChildByTag(3));
				list->removeAllChildrenWithCleanup(true);
				for (int i = 0; i < MessageLine; i++) {
					insertText(MsgBuffer[i]);
					//log("%s", a2u(MsgBuffer[i]));
					//MixedImage.DrawText(hFont, MsgX(0), MsgY(i), MsgBuffer[i]);
				}
			}
			else
			{
				_gridNodeTarget->getChildByTag(nodename)->getChildByTag(1)->getChildByTag(4)->setVisible(false);
			}
			// 等待按键的符号（使用▼)
			if (WaitMarkShowing && flags & TextWaitMark)
				_gridNodeTarget->getChildByTag(nodename)->getChildByTag(1)->getChildByTag(4)->setVisible(true);

		}
		if (MenuShow) {
			_gridNodeTarget->getChildByTag(nodename)->setVisible(true);
		}
	}
	
}


void CScene_main::SentNotyMessage(UINT type)
{
	_WindowProc(type);
}
//
// WM_CLOSE的处理
//
void CScene_main::OnClose()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#else
	Director::getInstance()->end();
#endif
}
void CScene_main::SetGameLoadAction()
{
	ShowLoadSaveMenu(true);
	//Action->Pause();
	//Action = new CGameSaveAction(this, Action);
	//SentNotyMessage(PosNType::PS_IDLE);
}
void CScene_main::ShowLoadSaveMenu(bool isSave)
{
	IsSaveMenu = isSave;
	SaveShow = true;
	for (int i = 0; i < PARAMS_MAX_SAVE; i++)
	{
		CParams	param;
		if (param.Load(i)){
			DataTitle[i].activate = true;
			sprintf(DataTitle[i].title, "%2d: %2d/%2d %2d:%02d", i + 1,
			param.save_month, param.save_date,
			param.save_hour, param.save_minute);
		}
		else
		{
			DataTitle[i].activate = IsSaveMenu ? true : false;
			sprintf(DataTitle[i].title, "%2d: -- no data --", i + 1);
		}
		DataTitle[i].res = DataTitle[i].activate ? "white" : "gray";
	}
	Invalidate(SaveRect);
	if (TextShow)
		Invalidate(TextRect);
	if (MenuShow)
		Invalidate(MenuRect);
	Update();
}
void CScene_main::HideWaitMark()
{
	if (WaitMarkShowing) {
		WaitMarkShowing = false;
		if (TextShow) {
			Mixing(TextRect, TextWaitMark);
			//CopyAndRepaint(WaitMarkRect);
		}
	}
}

bool CScene_main::Battle(const char *name)
{
	//CBattleAction *action = new CBattleAction(this, Action);
	//if (!action->Load(name)) {
	//	delete action;
	//	return false;
	//}
	//Action = action;
	SentNotyMessage(PosNType::PS_IDLE);	// 为存安全，发送空事件
	return true;
}