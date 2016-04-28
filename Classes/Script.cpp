#include "Script.h"
#include "Scene_main.h"
#include <string.h>
#include "TH_log.h"

//#include <assert.h>
//
// 构造函数
//
CScriptAction::CScriptAction(CScene_main *win, CAction *oldAction, int mode)
: CAction(win, oldAction),reader(0)
{
	// 命令表格(command table)的初始化
	cmd_table.insert(CmdTab("set", &CScriptAction::SetCmd));
	cmd_table.insert(CmdTab("calc", &CScriptAction::SetCmd));
	cmd_table.insert(CmdTab("text", &CScriptAction::TextCmd));
	cmd_table.insert(CmdTab("goto", &CScriptAction::GotoCmd));
	cmd_table.insert(CmdTab("if", &CScriptAction::IfCmd));
	cmd_table.insert(CmdTab("menu", &CScriptAction::MenuCmd));
	cmd_table.insert(CmdTab("click", &CScriptAction::ClickMenuCmd));
	cmd_table.insert(CmdTab("exec", &CScriptAction::ExecCmd));
	cmd_table.insert(CmdTab("load", &CScriptAction::LoadCmd));
	cmd_table.insert(CmdTab("update", &CScriptAction::UpdateCmd));
	cmd_table.insert(CmdTab("clear", &CScriptAction::ClearCmd));
	cmd_table.insert(CmdTab("music", &CScriptAction::MusicCmd));
	cmd_table.insert(CmdTab("stopm", &CScriptAction::StopmCmd));
	cmd_table.insert(CmdTab("wait", &CScriptAction::WaitCmd));
	cmd_table.insert(CmdTab("sound", &CScriptAction::SoundCmd));
	cmd_table.insert(CmdTab("fadein", &CScriptAction::FadeInCmd));
	cmd_table.insert(CmdTab("fadeout", &CScriptAction::FadeOutCmd));
	cmd_table.insert(CmdTab("wipein", &CScriptAction::WipeInCmd));
	cmd_table.insert(CmdTab("wipeout", &CScriptAction::WipeOutCmd));
	cmd_table.insert(CmdTab("cutin", &CScriptAction::CutInCmd));
	cmd_table.insert(CmdTab("cutout", &CScriptAction::CutOutCmd));
	cmd_table.insert(CmdTab("whitein", &CScriptAction::WhiteInCmd));
	cmd_table.insert(CmdTab("whiteout", &CScriptAction::WhiteOutCmd));
	cmd_table.insert(CmdTab("flash", &CScriptAction::FlashCmd));
	cmd_table.insert(CmdTab("shake", &CScriptAction::ShakeCmd));
	cmd_table.insert(CmdTab("mode", &CScriptAction::ModeCmd));
	cmd_table.insert(CmdTab("system", &CScriptAction::SystemCmd));
	cmd_table.insert(CmdTab("battle", &CScriptAction::BattleCmd));
	cmd_table.insert(CmdTab("end", &CScriptAction::EndCmd));

	status = Continue;
	Pressed = false;
	MenuSelect = -1;
	PlayMode = mode;

	delete reader;
	reader = 0;
}
//
// 析构函数
//
CScriptAction::~CScriptAction()
{
	delete reader;
}

//
// 错误事件的格式
//
char * CScriptAction::Format(const char *fmt, ...)
{
	static	char	tmp[256];

	//va_list	args;
	//va_start(args, fmt);
	//_vsnprintf(tmp, sizeof(tmp), fmt, args);
	//va_end(args);

	return tmp;
}

//
// 暂停
//
void CScriptAction::Pause()
{
	switch (status)
	{
	case WaitMenuDone:
		if (MenuSelect >= 0)
		{
			Parent->SelectMenu(MenuSelect, false);
			MenuSelect = -1;
		}
		break;
	default:
		break;
	}
}

//
// 解除暂停
//
void CScriptAction::Resume()
{
	
}

//
// 按下鼠标左键时的处理
//
void CScriptAction::LButtonDown(UINT modKeys, Vec2 point)
{
	switch (status) {
	case WaitKeyPressed:	// 等待按键
		Parent->HideWaitMark();
		status = Continue;
		break;
	case WaitMenuDone:	// 等待菜单
	case WaitClickMenuDone:
		Pressed = true;
		break;
	}
}

//
// 放开鼠标左键时的处理
//
void CScriptAction::LButtonUp(UINT modKeys, Vec2 point)
{
	switch (status) {
	case WaitKeyPressed:	// 等待按键
		//Parent->HideWaitMark();
		status = Continue;
		break;

	case WaitMenuDone:	// 等待菜单
		if (Pressed) {
			Pressed = false;
			//MouseMove(modKeys, point);

			if (MenuSelect >= 0) {
				Parent->SetValue(MenuAnser, Parent->GetMenuAnser(MenuSelect));
				Parent->HideMenuWindow();
				status = Continue;
			}
			MenuSelect = -1;
		}
		break;

	case WaitClickMenuDone:
		if (Pressed) {
			Pressed = false;
			//int sel = Parent->GetClickMenuSelect(point);
			//if (sel >= 0) {
			//	Parent->SetValue(MenuAnser, sel);
			//	status = Continue;
		//	}
		}
		break;
	}
}

//
// 按下鼠标右键时的处理
//
void CScriptAction::RButtonDown(UINT modKeys, Vec2 point)
{
	
}

//
// 移动鼠标时的处理
//
void CScriptAction::MouseMove(UINT modKeys, Vec2 point)
{
	//switch (status) {
	//case WaitMenuDone:	// 等待菜单
	//{
	//						int sel = Parent->GetMenuSelect(point);
	//						if (sel != MenuSelect) {
	//							Parent->SelectMenu(MenuSelect, false);
	//							MenuSelect = sel;
	//							Parent->SelectMenu(MenuSelect, true);
	//						}
	//}
	//	break;
	//}
}

//
// 按下键盘时的处理
//
void CScriptAction::KeyDown(UINT key)
{
	
}

//
// IDLE的处理
//
bool CScriptAction::IdleAction()
{
	if (status == Continue) {				// 执行“继续”
		do {
			status = Step();				// 执行一步
		} while (status == Continue);		// 继续吗?

		if (status == BreakGame) {			// 结束
			Abort();
		}
		else if (status == WaitNextIdle) {	// 等待下个IDLE处理
			status = Continue;				// 继续
			return true;
		}
		else if (status == WaitWipeDone) {	// 等待特效结束
			return true;					// IDLE继续
		}
	}
	return false;
}

//
// 计时器的处理
//
bool CScriptAction::TimedOut(int timerId)
{
	
	return true;
}

//
// Wipe特效结束时的处理
//
void CScriptAction::WipeDone()
{
	if (status == WaitWipeDone)		// 等待特效结束
		status = Continue;
}

//
// Wave播放结束时的处理
//
void CScriptAction::WaveDone()
{
	if (status == WaitWaveDone)		// 等待WAVE播放结束
		status = Continue;
}

//
// Script执行结束
//
void CScriptAction::Abort()
{
	
}

//
// 读取Script档
//
bool CScriptAction::LoadFile(const char *name)
{
	
	char path[260];
	sprintf(path, SCRIPTPATH "%s.txt", name);
	label_table.clear();
	reader = new TextReader(path);
	return reader->IsOk();
}

//
// 读取Script档，并设定成员变量
//
bool CScriptAction::Load(const char *name)
{
	strncpy(Parent->GetParam().last_script, name, 16);
	if (!LoadFile(name))
	{
		//读取script错误
		return false;
	}
	return true;
}

//
// 进行设定，让Script从储存位置中可以再度开启
//
//bool CScriptAction::Setup(CParams &param)
//{
//	
//	return true;
//}

//
// 错误事件的输出
//
void  CScriptAction::Error(const char *fmt, ...)
{
	va_list	args;

	char	str[256];
	int len = 0;
	THLog_Write(LOG_ID_GAME, true, fmt);
	//if (reader)
	//	len = sprintf(str, "%s:%d ", reader->GetFileName(), reader->GetLineNo());
	//va_start(args, fmt);
	//_vsnprintf(str + len, sizeof(str)-len, fmt, args);
	//va_end(args);

	//Parent->MessageBox(str);
}

//
// 比对关键字
//
bool CScriptAction::ChkKeyword(const char *str, const char *keyword)
{
	while (*str) {
		if (tolower(*str++) != *keyword++)
			return false;
	}
	return true;
}

//
// CG的指定位置设定成位置码
//
int CScriptAction::GetPosition(const char *str)
{
	if (ChkKeyword(str, "bg") || ChkKeyword(str, "back"))
		return POSITION_BACK;
	if (ChkKeyword(str, "bgo") || ChkKeyword(str, "backonly"))
		return POSITION_BACKONLY;
	if (ChkKeyword(str, "ov"))
		return POSITION_OVERLAP;
	if (ChkKeyword(str, "work"))
		return POSITION_WORK;

	Error("文法错误(position)");
	return POSITION_BACK;
}

//
// 依照命令来更新指定特效
//
int CScriptAction::GetUpdateType(const char *str)
{
	if (ChkKeyword(str, "cut") || ChkKeyword(str, "now"))
		return UPDATE_NOW;
	if (ChkKeyword(str, "overlap"))
		return UPDATE_OVERLAP;
	if (ChkKeyword(str, "wipe"))
		return UPDATE_WIPE;

	Error("文法错误(update type)");
	return UPDATE_NOW;
}

//
// 移到Script中指定的位置
//
int CScriptAction::GotoCommand(const char *label)
{
	labelmap::iterator p = label_table.find(label);
	if (p == label_table.end()) {		// 找不到标签
		const char *str;
		// 从Script档中搜寻标签
		while ((str = reader->GetString()) != NULL) {
			Lexer	lexer(str);
			if (lexer.NumToken() == 1
				&& lexer.GetType() == Lexer::IsLabel) {
				if (SetLabel(lexer) != Continue)	// 登录标签
					return BreakGame;

				const char *p = lexer.GetString(0) + 1;
				if (C_STRICMP(p, label) == 0)			// 找到标签了！
					return Continue;
			}
		}
		Error("找不到标签[%s]", label);
		return BreakGame;
	}
	reader->SetPosition(p->second);

	return Continue;
}

//
// 标签的登录指令
//
int CScriptAction::SetLabel(Lexer &lexer)
{
	if (lexer.NumToken() != 1)
	{
		cocos2d::log("参数太多了");
		return BreakGame;
	}
	const char *label = lexer.GetString() + 1;
	labelmap::iterator p = label_table.find(label);
	if (p != label_table.end()) {	// 标签已经存在
		if (label_table[label] != reader->GetPosition()) {
			cocos2d::log("标签 [%s] 已经登录过了", label);
			return BreakGame;
		}
	}
	label_table[label] = reader->GetPosition();
	return Continue;
}

//
// set指令
//
int CScriptAction::SetCmd(Lexer &lexer)
{
	
	return Continue;
}

//
// goto指令
//
int CScriptAction::GotoCmd(Lexer &lexer)
{
	const char *label = lexer.GetString();

	if (label == 0 || lexer.GetString() != 0) {
		Error("文法错误(in goto command)");
		return BreakGame;
	}
	return GotoCommand(label);
}

//
// 取得变量或常数
//
bool CScriptAction::GetValue(Lexer &lexer, int *value)
{
	if (lexer.GetType() == Lexer::IsString) {	// 字符串
		const char *name = lexer.GetString();
		*value = Parent->GetValue(name);
		return true;
	}
	return lexer.GetValue(value);		// 当成数字读看看
}

//
// if 指令
//
int CScriptAction::IfCmd(Lexer &lexer)
{
	int	value1, value2;
	bool	b1 = GetValue(lexer, &value1);
	const char *op = lexer.GetString();
	bool	b2 = GetValue(lexer, &value2);
	const char *p = lexer.GetString();
	const char *label = lexer.GetString();

	if (!b1 || !b2 || op == 0 || label == 0
		|| lexer.GetString() != 0 && C_STRICMP(p, "goto") != 0) {
		Error("文法错误");
		return BreakGame;
	}

	if (strcmp(op, "==") == 0) {
		if (value1 == value2)
			return GotoCommand(label);
	}
	else if (strcmp(op, "!=") == 0) {
		if (value1 != value2)
			return GotoCommand(label);
	}
	else if (strcmp(op, "<=") == 0) {
		if (value1 <= value2)
			return GotoCommand(label);
	}
	else if (strcmp(op, ">=") == 0) {
		if (value1 >= value2)
			return GotoCommand(label);
	}
	else if (strcmp(op, "<") == 0) {
		if (value1 < value2)
			return GotoCommand(label);
	}
	else if (strcmp(op, ">") == 0) {
		if (value1 > value2)
			return GotoCommand(label);
	}
	else {
		Error("文法错误");
		return BreakGame;
	}
	return Continue;
}

//
// menu 指令
//
int CScriptAction::MenuCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();
	if (p == 0 || lexer.GetString() != 0) {
		cocos2d::log("文法错误(in menu command)");
		return BreakGame;
	}
	Parent->ClearMenuItemCount();
	MenuAnser = p;
	const char *str;
	for (int no = 0; (str = reader->GetString()) != NULL; no++) {


//#if defined(_WIN32)
		if (C_STRICMP(str, "end") == 0)
			break;
		
		Parent->SetMenuItem(str, no + 1);
	}
	MenuSelect = -1;
	Parent->OpenMenu();
	return WaitMenuDone;
}

//
// click命令
//
int CScriptAction::ClickMenuCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();
	return WaitClickMenuDone;
}

//
// load 指令
//
int CScriptAction::LoadCmd(Lexer &lexer)
{
	const char *p1 = lexer.GetString();
	const char *p2 = lexer.GetString();

	if (p1 == 0 || p2 == 0 || lexer.GetString() != 0) {
		Error("文法错误(in load command)");
		return BreakGame;
	}
	return LoadGraphic(p2, GetPosition(p1));
}

//
// update 指令
//
int CScriptAction::UpdateCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();

	if (p == 0 || lexer.GetString() != 0) {
		Error("文法错误(in update command)");
		return BreakGame;
	}
	return UpdateImage(GetUpdateType(p));
}

//
// clear 指令
//
int CScriptAction::ClearCmd(Lexer &lexer)
{
	
	const char *p = lexer.GetString();

	if (p == 0 || lexer.GetString() != 0) {
		Error("文法错误(in clear command)");
		return BreakGame;
	}

	if (C_STRICMP(p, "text") == 0) {		// clear text 个别处理
		Parent->ClearMessage();
		return WaitNextIdle;
	}
	return Clear(GetPosition(p));
}

//
// music 指令
//
int CScriptAction::MusicCmd(Lexer &lexer)
{
	
	return Continue;
}

//
// sound 指令
//
int CScriptAction::SoundCmd(Lexer &lexer)
{
	
	return Continue;
}

//
// exec 指令
//
int CScriptAction::ExecCmd(Lexer &lexer)
{
	
	const char *path = lexer.GetString();

	if (path == 0 || lexer.GetString() != 0) {
		Error("文法错误(in exec command)");
		return BreakGame;
	}
	if (!Load(path))
		return BreakGame;
	PlayMode = MODE_SCENARIO;
	return Continue;
}

//
// wait 指令
//
int CScriptAction::WaitCmd(Lexer &lexer)
{
	
	return WaitTimeOut;
}

//
// Text的尾端确认
//
bool CScriptAction::ChkTermination(const char *str)
{
	if (*str++ != '.')
		return false;
	while (*str) {
		if (!isspace(*str))
			return false;
	}
	return true;
}

//
// text 指令
//
int CScriptAction::TextCmd(Lexer &lexer)
{
	if (lexer.GetString() != 0) {
		Error("文法错误(in text command)");
		return BreakGame;
	}
	std::string	work;
	for (int i = 0;; i++) {
		const char *str;
		if ((str = reader->GetString()) == nullptr) {
			Error("文法错误(text syntax)");
			return BreakGame;
		}
		if (ChkTermination(str))
			break;
		work += str;
		work += '\n';

		if (i >= MessageLine) {
			Error("内容行数过多");
			return BreakGame;
		}
	}
	Parent->WriteMessage(work.c_str());
	return WaitKeyPressed;
}

//
// mode 指令
//
int CScriptAction::ModeCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();
	if (p == 0 || lexer.GetString() != 0) {
		Error("文法错误");
		return BreakGame;
	}

//#if defined(_WIN32)
	if (C_STRICMP(p, "system") == 0){
		PlayMode = MODE_SYSTEM;
	}
	else if (C_STRICMP(p, "scenario") == 0) {
		PlayMode = MODE_SCENARIO;
	}
	else {
		Error("文法错误");
		return BreakGame;
	}

	
	return Continue;
}

//
// system 指令
//
int CScriptAction::SystemCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();
	if (p == 0 || lexer.GetString() != 0) {
		Error("文法错误");
		return BreakGame;
	}
	if (C_STRICMP(p, "load") == 0) {
		Parent->SetGameLoadAction();
		return WaitNextIdle;
	}
	if (C_STRICMP(p, "exit") == 0) {
		Parent->SentNotyMessage(PS_CLOSE);
		return WaitNextIdle;
	}
	if (C_STRICMP(p, "clear") == 0) {
		Parent->ClearValue();
		return Continue;
	}
	Error("文法错误");
	return BreakGame;
}

//
// battle 指令
//
int CScriptAction::BattleCmd(Lexer &lexer)
{

	const char *p = lexer.GetString();
	if (p == 0 || lexer.GetString() != 0) {
		Error("文法错误");
		return BreakGame;
	}
	Parent->Battle(p);
	return WaitNextIdle;
}

//
// stopm 指令
//
int CScriptAction::StopmCmd(Lexer &lexer)
{

	return Continue;
}

//
// fadein 指令
//
int CScriptAction::FadeInCmd(Lexer &lexer)
{
	Parent->GetParam().SetShowFlag();
	Parent->FadeIn();
	return WaitWipeDone;
}

//
// fadeout 指令
//
int CScriptAction::FadeOutCmd(Lexer &lexer)
{
	Parent->GetParam().ResetShowFlag();
	Parent->FadeOut();
	return WaitWipeDone;
}

//
// wipein 指令
//
int CScriptAction::WipeInCmd(Lexer &lexer)
{
	Parent->GetParam().SetShowFlag();
	Parent->WipeIn();
	return WaitWipeDone;
}

//
// wipeout 指令
//
int CScriptAction::WipeOutCmd(Lexer &lexer)
{
	Parent->GetParam().SetShowFlag();
	Parent->WipeOut();
	return WaitWipeDone;
}

//
// cutin 指令
//
int CScriptAction::CutInCmd(Lexer &lexer)
{

	return WaitNextIdle;
}

//
// cutout 指令
//
int CScriptAction::CutOutCmd(Lexer &lexer)
{

	return WaitNextIdle;
}

//
// whitein 指令
//
int CScriptAction::WhiteInCmd(Lexer &lexer)
{

	return WaitWipeDone;
}

//
// whiteout 指令
//
int CScriptAction::WhiteOutCmd(Lexer &lexer)
{
	return WaitWipeDone;
}

//
// flash 指令
//
int CScriptAction::FlashCmd(Lexer &lexer)
{
	return WaitWipeDone;
}

//
// shake 指令
//
int CScriptAction::ShakeCmd(Lexer &lexer)
{
	return WaitWipeDone;
}

//
// end 指令
//
int CScriptAction::EndCmd(Lexer &lexer)
{
	return BreakGame;
}

//
// 指令判定
//
CScriptAction::cmd_t CScriptAction::ParseCommand(Lexer &lexer)
{
	const char *cmd = lexer.GetString(0);
	cmdmap::iterator	p = cmd_table.find(cmd);
	if (p != cmd_table.end())
		return p->second;

	// set, calc的省略型吗？
	if (lexer.NumToken() >= 3) {
		const char *p = lexer.GetString(1);
		lexer.GetType(0);			// 回到最前面
		if (strcmp(p, "+") == 0 || strcmp(p, "-") == 0 || strcmp(p, "=") == 0) {
			return &CScriptAction::SetCmd;
		}
	}
	Error("文法错误(command syntax)");
	return NULL;
}

//
// 解析一行
//
int CScriptAction::ParserString(const char *str)
{
	Lexer	lexer(str);

	if (lexer.NumToken() == 0)
		return Continue;

	int	type = lexer.GetType();

	if (type == Lexer::IsLabel) {
		return SetLabel(lexer);
	}
	cmd_t cmd = ParseCommand(lexer);
	if (cmd == 0)
		return BreakGame;
	return (this->*cmd)(lexer);
}

//
// 执行Script的一个步骤
//
int CScriptAction::Step()
{
	CC_ASSERT(reader);
	// 将现在的位置设定成SaveDate todo

	const char *str = reader->GetString();
	if (str == 0)
		return BreakGame;
	return ParserString(str);
}


//
// CG的载入
//
int CScriptAction::LoadGraphic(const char *file, int pos)
{
	bool	result = false;
	switch (pos) {
	case POSITION_BACK:			// 背景
		Parent->GetParam().ClearOverlapCG();
		Parent->ClearOverlap();
		// no break

	case POSITION_BACKONLY:		// 背景的部分
		Parent->GetParam().SetBackCG(file);
		result = Parent->LoadImageBack(file);
		break;

	case POSITION_OVERLAP:		// 重叠
		Parent->GetParam().SetOverlapCG(file);
		result = Parent->LoadImageOverlap(file);
		break;

	case POSITION_WORK:			// 作业使用
		Parent->GetParam().SetWorkCG(file);
		result = Parent->LoadImageWork(file);
		break;
	}

	if (!result) {
		//Parent->MessageBox(Format("档案无法读出 [%s] 档案", file));
		if (PlayMode == MODE_SYSTEM)
		{
			//离开游戏
			//Parent->SendMessage(WM_CLOSE);
		}

		return BreakGame;
	}
	return Continue;
}


//
// 删除CG
//
int CScriptAction::Clear(int pos)
{
	switch (pos) {
	case POSITION_BACK:			// 背景
		Parent->GetParam().ClearOverlapCG();
		Parent->ClearOverlap();
		// no break

	case POSITION_BACKONLY:		// 背景的部分
		Parent->GetParam().ClearBackCG();
		Parent->ClearBack();
		break;

	case POSITION_OVERLAP:		// 重叠
		Parent->GetParam().ClearOverlapCG();
		Parent->ClearOverlap();
		break;

	case POSITION_WORK:			// 作业使用
		Parent->GetParam().ClearWorkCG();
		break;
	}
	return Continue;
}


//
// 画面更新
//
int CScriptAction::UpdateImage(int flag)
{
	Parent->GetParam().SetShowFlag();

	auto rect = Parent->GetInvalidRect();
	if (rect==0)
		return Continue;

	switch (flag) {
	case UPDATE_NOW:
		Parent->CutIn(rect);
		return WaitNextIdle;

	case UPDATE_OVERLAP:
		Parent->MixFade(rect);
		break;

	case UPDATE_WIPE:
		Parent->WipeIn(rect);
		break;
	}
	return WaitWipeDone;
}

