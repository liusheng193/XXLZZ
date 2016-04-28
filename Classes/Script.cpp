#include "Script.h"
#include "Scene_main.h"
#include <string.h>
#include "TH_log.h"

//#include <assert.h>
//
// ���캯��
//
CScriptAction::CScriptAction(CScene_main *win, CAction *oldAction, int mode)
: CAction(win, oldAction),reader(0)
{
	// ������(command table)�ĳ�ʼ��
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
// ��������
//
CScriptAction::~CScriptAction()
{
	delete reader;
}

//
// �����¼��ĸ�ʽ
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
// ��ͣ
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
// �����ͣ
//
void CScriptAction::Resume()
{
	
}

//
// ����������ʱ�Ĵ���
//
void CScriptAction::LButtonDown(UINT modKeys, Vec2 point)
{
	switch (status) {
	case WaitKeyPressed:	// �ȴ�����
		Parent->HideWaitMark();
		status = Continue;
		break;
	case WaitMenuDone:	// �ȴ��˵�
	case WaitClickMenuDone:
		Pressed = true;
		break;
	}
}

//
// �ſ�������ʱ�Ĵ���
//
void CScriptAction::LButtonUp(UINT modKeys, Vec2 point)
{
	switch (status) {
	case WaitKeyPressed:	// �ȴ�����
		//Parent->HideWaitMark();
		status = Continue;
		break;

	case WaitMenuDone:	// �ȴ��˵�
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
// ��������Ҽ�ʱ�Ĵ���
//
void CScriptAction::RButtonDown(UINT modKeys, Vec2 point)
{
	
}

//
// �ƶ����ʱ�Ĵ���
//
void CScriptAction::MouseMove(UINT modKeys, Vec2 point)
{
	//switch (status) {
	//case WaitMenuDone:	// �ȴ��˵�
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
// ���¼���ʱ�Ĵ���
//
void CScriptAction::KeyDown(UINT key)
{
	
}

//
// IDLE�Ĵ���
//
bool CScriptAction::IdleAction()
{
	if (status == Continue) {				// ִ�С�������
		do {
			status = Step();				// ִ��һ��
		} while (status == Continue);		// ������?

		if (status == BreakGame) {			// ����
			Abort();
		}
		else if (status == WaitNextIdle) {	// �ȴ��¸�IDLE����
			status = Continue;				// ����
			return true;
		}
		else if (status == WaitWipeDone) {	// �ȴ���Ч����
			return true;					// IDLE����
		}
	}
	return false;
}

//
// ��ʱ���Ĵ���
//
bool CScriptAction::TimedOut(int timerId)
{
	
	return true;
}

//
// Wipe��Ч����ʱ�Ĵ���
//
void CScriptAction::WipeDone()
{
	if (status == WaitWipeDone)		// �ȴ���Ч����
		status = Continue;
}

//
// Wave���Ž���ʱ�Ĵ���
//
void CScriptAction::WaveDone()
{
	if (status == WaitWaveDone)		// �ȴ�WAVE���Ž���
		status = Continue;
}

//
// Scriptִ�н���
//
void CScriptAction::Abort()
{
	
}

//
// ��ȡScript��
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
// ��ȡScript�������趨��Ա����
//
bool CScriptAction::Load(const char *name)
{
	strncpy(Parent->GetParam().last_script, name, 16);
	if (!LoadFile(name))
	{
		//��ȡscript����
		return false;
	}
	return true;
}

//
// �����趨����Script�Ӵ���λ���п����ٶȿ���
//
//bool CScriptAction::Setup(CParams &param)
//{
//	
//	return true;
//}

//
// �����¼������
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
// �ȶԹؼ���
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
// CG��ָ��λ���趨��λ����
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

	Error("�ķ�����(position)");
	return POSITION_BACK;
}

//
// ��������������ָ����Ч
//
int CScriptAction::GetUpdateType(const char *str)
{
	if (ChkKeyword(str, "cut") || ChkKeyword(str, "now"))
		return UPDATE_NOW;
	if (ChkKeyword(str, "overlap"))
		return UPDATE_OVERLAP;
	if (ChkKeyword(str, "wipe"))
		return UPDATE_WIPE;

	Error("�ķ�����(update type)");
	return UPDATE_NOW;
}

//
// �Ƶ�Script��ָ����λ��
//
int CScriptAction::GotoCommand(const char *label)
{
	labelmap::iterator p = label_table.find(label);
	if (p == label_table.end()) {		// �Ҳ�����ǩ
		const char *str;
		// ��Script������Ѱ��ǩ
		while ((str = reader->GetString()) != NULL) {
			Lexer	lexer(str);
			if (lexer.NumToken() == 1
				&& lexer.GetType() == Lexer::IsLabel) {
				if (SetLabel(lexer) != Continue)	// ��¼��ǩ
					return BreakGame;

				const char *p = lexer.GetString(0) + 1;
				if (C_STRICMP(p, label) == 0)			// �ҵ���ǩ�ˣ�
					return Continue;
			}
		}
		Error("�Ҳ�����ǩ[%s]", label);
		return BreakGame;
	}
	reader->SetPosition(p->second);

	return Continue;
}

//
// ��ǩ�ĵ�¼ָ��
//
int CScriptAction::SetLabel(Lexer &lexer)
{
	if (lexer.NumToken() != 1)
	{
		cocos2d::log("����̫����");
		return BreakGame;
	}
	const char *label = lexer.GetString() + 1;
	labelmap::iterator p = label_table.find(label);
	if (p != label_table.end()) {	// ��ǩ�Ѿ�����
		if (label_table[label] != reader->GetPosition()) {
			cocos2d::log("��ǩ [%s] �Ѿ���¼����", label);
			return BreakGame;
		}
	}
	label_table[label] = reader->GetPosition();
	return Continue;
}

//
// setָ��
//
int CScriptAction::SetCmd(Lexer &lexer)
{
	
	return Continue;
}

//
// gotoָ��
//
int CScriptAction::GotoCmd(Lexer &lexer)
{
	const char *label = lexer.GetString();

	if (label == 0 || lexer.GetString() != 0) {
		Error("�ķ�����(in goto command)");
		return BreakGame;
	}
	return GotoCommand(label);
}

//
// ȡ�ñ�������
//
bool CScriptAction::GetValue(Lexer &lexer, int *value)
{
	if (lexer.GetType() == Lexer::IsString) {	// �ַ���
		const char *name = lexer.GetString();
		*value = Parent->GetValue(name);
		return true;
	}
	return lexer.GetValue(value);		// �������ֶ�����
}

//
// if ָ��
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
		Error("�ķ�����");
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
		Error("�ķ�����");
		return BreakGame;
	}
	return Continue;
}

//
// menu ָ��
//
int CScriptAction::MenuCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();
	if (p == 0 || lexer.GetString() != 0) {
		cocos2d::log("�ķ�����(in menu command)");
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
// click����
//
int CScriptAction::ClickMenuCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();
	return WaitClickMenuDone;
}

//
// load ָ��
//
int CScriptAction::LoadCmd(Lexer &lexer)
{
	const char *p1 = lexer.GetString();
	const char *p2 = lexer.GetString();

	if (p1 == 0 || p2 == 0 || lexer.GetString() != 0) {
		Error("�ķ�����(in load command)");
		return BreakGame;
	}
	return LoadGraphic(p2, GetPosition(p1));
}

//
// update ָ��
//
int CScriptAction::UpdateCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();

	if (p == 0 || lexer.GetString() != 0) {
		Error("�ķ�����(in update command)");
		return BreakGame;
	}
	return UpdateImage(GetUpdateType(p));
}

//
// clear ָ��
//
int CScriptAction::ClearCmd(Lexer &lexer)
{
	
	const char *p = lexer.GetString();

	if (p == 0 || lexer.GetString() != 0) {
		Error("�ķ�����(in clear command)");
		return BreakGame;
	}

	if (C_STRICMP(p, "text") == 0) {		// clear text ������
		Parent->ClearMessage();
		return WaitNextIdle;
	}
	return Clear(GetPosition(p));
}

//
// music ָ��
//
int CScriptAction::MusicCmd(Lexer &lexer)
{
	
	return Continue;
}

//
// sound ָ��
//
int CScriptAction::SoundCmd(Lexer &lexer)
{
	
	return Continue;
}

//
// exec ָ��
//
int CScriptAction::ExecCmd(Lexer &lexer)
{
	
	const char *path = lexer.GetString();

	if (path == 0 || lexer.GetString() != 0) {
		Error("�ķ�����(in exec command)");
		return BreakGame;
	}
	if (!Load(path))
		return BreakGame;
	PlayMode = MODE_SCENARIO;
	return Continue;
}

//
// wait ָ��
//
int CScriptAction::WaitCmd(Lexer &lexer)
{
	
	return WaitTimeOut;
}

//
// Text��β��ȷ��
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
// text ָ��
//
int CScriptAction::TextCmd(Lexer &lexer)
{
	if (lexer.GetString() != 0) {
		Error("�ķ�����(in text command)");
		return BreakGame;
	}
	std::string	work;
	for (int i = 0;; i++) {
		const char *str;
		if ((str = reader->GetString()) == nullptr) {
			Error("�ķ�����(text syntax)");
			return BreakGame;
		}
		if (ChkTermination(str))
			break;
		work += str;
		work += '\n';

		if (i >= MessageLine) {
			Error("������������");
			return BreakGame;
		}
	}
	Parent->WriteMessage(work.c_str());
	return WaitKeyPressed;
}

//
// mode ָ��
//
int CScriptAction::ModeCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();
	if (p == 0 || lexer.GetString() != 0) {
		Error("�ķ�����");
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
		Error("�ķ�����");
		return BreakGame;
	}

	
	return Continue;
}

//
// system ָ��
//
int CScriptAction::SystemCmd(Lexer &lexer)
{
	const char *p = lexer.GetString();
	if (p == 0 || lexer.GetString() != 0) {
		Error("�ķ�����");
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
	Error("�ķ�����");
	return BreakGame;
}

//
// battle ָ��
//
int CScriptAction::BattleCmd(Lexer &lexer)
{

	const char *p = lexer.GetString();
	if (p == 0 || lexer.GetString() != 0) {
		Error("�ķ�����");
		return BreakGame;
	}
	Parent->Battle(p);
	return WaitNextIdle;
}

//
// stopm ָ��
//
int CScriptAction::StopmCmd(Lexer &lexer)
{

	return Continue;
}

//
// fadein ָ��
//
int CScriptAction::FadeInCmd(Lexer &lexer)
{
	Parent->GetParam().SetShowFlag();
	Parent->FadeIn();
	return WaitWipeDone;
}

//
// fadeout ָ��
//
int CScriptAction::FadeOutCmd(Lexer &lexer)
{
	Parent->GetParam().ResetShowFlag();
	Parent->FadeOut();
	return WaitWipeDone;
}

//
// wipein ָ��
//
int CScriptAction::WipeInCmd(Lexer &lexer)
{
	Parent->GetParam().SetShowFlag();
	Parent->WipeIn();
	return WaitWipeDone;
}

//
// wipeout ָ��
//
int CScriptAction::WipeOutCmd(Lexer &lexer)
{
	Parent->GetParam().SetShowFlag();
	Parent->WipeOut();
	return WaitWipeDone;
}

//
// cutin ָ��
//
int CScriptAction::CutInCmd(Lexer &lexer)
{

	return WaitNextIdle;
}

//
// cutout ָ��
//
int CScriptAction::CutOutCmd(Lexer &lexer)
{

	return WaitNextIdle;
}

//
// whitein ָ��
//
int CScriptAction::WhiteInCmd(Lexer &lexer)
{

	return WaitWipeDone;
}

//
// whiteout ָ��
//
int CScriptAction::WhiteOutCmd(Lexer &lexer)
{
	return WaitWipeDone;
}

//
// flash ָ��
//
int CScriptAction::FlashCmd(Lexer &lexer)
{
	return WaitWipeDone;
}

//
// shake ָ��
//
int CScriptAction::ShakeCmd(Lexer &lexer)
{
	return WaitWipeDone;
}

//
// end ָ��
//
int CScriptAction::EndCmd(Lexer &lexer)
{
	return BreakGame;
}

//
// ָ���ж�
//
CScriptAction::cmd_t CScriptAction::ParseCommand(Lexer &lexer)
{
	const char *cmd = lexer.GetString(0);
	cmdmap::iterator	p = cmd_table.find(cmd);
	if (p != cmd_table.end())
		return p->second;

	// set, calc��ʡ������
	if (lexer.NumToken() >= 3) {
		const char *p = lexer.GetString(1);
		lexer.GetType(0);			// �ص���ǰ��
		if (strcmp(p, "+") == 0 || strcmp(p, "-") == 0 || strcmp(p, "=") == 0) {
			return &CScriptAction::SetCmd;
		}
	}
	Error("�ķ�����(command syntax)");
	return NULL;
}

//
// ����һ��
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
// ִ��Script��һ������
//
int CScriptAction::Step()
{
	CC_ASSERT(reader);
	// �����ڵ�λ���趨��SaveDate todo

	const char *str = reader->GetString();
	if (str == 0)
		return BreakGame;
	return ParserString(str);
}


//
// CG������
//
int CScriptAction::LoadGraphic(const char *file, int pos)
{
	bool	result = false;
	switch (pos) {
	case POSITION_BACK:			// ����
		Parent->GetParam().ClearOverlapCG();
		Parent->ClearOverlap();
		// no break

	case POSITION_BACKONLY:		// �����Ĳ���
		Parent->GetParam().SetBackCG(file);
		result = Parent->LoadImageBack(file);
		break;

	case POSITION_OVERLAP:		// �ص�
		Parent->GetParam().SetOverlapCG(file);
		result = Parent->LoadImageOverlap(file);
		break;

	case POSITION_WORK:			// ��ҵʹ��
		Parent->GetParam().SetWorkCG(file);
		result = Parent->LoadImageWork(file);
		break;
	}

	if (!result) {
		//Parent->MessageBox(Format("�����޷����� [%s] ����", file));
		if (PlayMode == MODE_SYSTEM)
		{
			//�뿪��Ϸ
			//Parent->SendMessage(WM_CLOSE);
		}

		return BreakGame;
	}
	return Continue;
}


//
// ɾ��CG
//
int CScriptAction::Clear(int pos)
{
	switch (pos) {
	case POSITION_BACK:			// ����
		Parent->GetParam().ClearOverlapCG();
		Parent->ClearOverlap();
		// no break

	case POSITION_BACKONLY:		// �����Ĳ���
		Parent->GetParam().ClearBackCG();
		Parent->ClearBack();
		break;

	case POSITION_OVERLAP:		// �ص�
		Parent->GetParam().ClearOverlapCG();
		Parent->ClearOverlap();
		break;

	case POSITION_WORK:			// ��ҵʹ��
		Parent->GetParam().ClearWorkCG();
		break;
	}
	return Continue;
}


//
// �������
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

