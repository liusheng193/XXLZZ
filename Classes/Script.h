#ifndef __script_h__
#define __script_h__
#include "TextReader.h"
#include "Lexer.h"
#include "less.h"
#include "action.h"
//
// Script class
//
class CScene_main;

class CScriptAction : public CAction
{
protected:
	// 状态
	enum	{
		BreakGame = -1,
		Continue = 0,
		WaitNextIdle,
		WaitKeyPressed,
		WaitTimeOut,
		WaitMenuDone,
		WaitClickMenuDone,
		WaitWipeDone,
		WaitWaveDone,
	};

	// CG位置
	enum	{
		POSITION_BACK,
		POSITION_BACKONLY,
		POSITION_OVERLAP,
		POSITION_WORK,
	};

	// Update Code
	enum	{
		UPDATE_NOW,
		UPDATE_OVERLAP,
		UPDATE_WIPE,
	};

public:
	// Mode flag
	enum	{
		MODE_SYSTEM,
		MODE_SCENARIO,
	};
protected:
	typedef int(CScriptAction::*cmd_t)(Lexer &);
	typedef pair<const char *, cmd_t>	CmdTab;
	typedef map<const char *, cmd_t, ic_less> cmdmap;
	typedef map<string, int, ics_less>	labelmap;
public:
	CScriptAction(CScene_main *win, CAction *oldAction, int mode);
	~CScriptAction();
	virtual	void Pause();
	virtual	void Resume();

	virtual	void LButtonDown(UINT modKeys, cocos2d::Vec2 point);
	virtual	void LButtonUp(UINT modKeys, cocos2d::Vec2 point);
	virtual	void RButtonDown(UINT modKeys, cocos2d::Vec2 point);
	virtual	void MouseMove(UINT modKeys, cocos2d::Vec2 point);

	virtual	void KeyDown(UINT key);
	virtual bool TimedOut(int timerId);

	virtual bool IdleAction();
	virtual void WipeDone();
	virtual void WaveDone();

	virtual bool IsSaveLoadOK() const;
	virtual void Abort();

//	bool Setup(CParams &param);
	bool Load(const char *name);
	void SetMenuSelect(int sel){ this->MenuSelect = sel; };
protected:
	char * Format(const char *fmt, ...);
	bool LoadFile(const char *name);
	int GotoCommand(const char *label);
	int LoadGraphic(const char *file, int pos);
	int UpdateImage(int flag);
	int Clear(int pos);

	int Step();
	cmd_t ParseCommand(Lexer &lexer);
	int ParserString(const char *str);

	int SetLabel(Lexer &lexer);
	int SetCmd(Lexer &lexer);
	int GotoCmd(Lexer &lexer);
	int IfCmd(Lexer &lexer);
	int MenuCmd(Lexer &lexer);
	int ClickMenuCmd(Lexer &lexer);
	int ExecCmd(Lexer &lexer);
	int LoadCmd(Lexer &lexer);
	int UpdateCmd(Lexer &lexer);
	int ClearCmd(Lexer &lexer);
	int WaitCmd(Lexer &lexer);
	int MusicCmd(Lexer &lexer);
	int SoundCmd(Lexer &lexer);
	int TextCmd(Lexer &lexer);
	int ModeCmd(Lexer &lexer);
	int SystemCmd(Lexer &lexer);
	int StopmCmd(Lexer &lexer);
	int FadeInCmd(Lexer &lexer);
	int FadeOutCmd(Lexer &lexer);
	int WipeInCmd(Lexer &lexer);
	int WipeOutCmd(Lexer &lexer);
	int CutInCmd(Lexer &lexer);
	int CutOutCmd(Lexer &lexer);
	int WhiteInCmd(Lexer &lexer);
	int WhiteOutCmd(Lexer &lexer);
	int FlashCmd(Lexer &lexer);
	int ShakeCmd(Lexer &lexer);
	int BattleCmd(Lexer &lexer);
	int EndCmd(Lexer &lexer);

	void  Error(const char *fmt, ...);

	bool GetValue(Lexer &lexer, int *value);
	bool ChkTermination(const char *str);
	bool ChkKeyword(const char *str, const char *keyword);
	int GetPosition(const char *str);
	int GetUpdateType(const char *str);

protected:
	bool		Pressed;
	int			MenuSelect;
	int			PlayMode;
	string		MenuAnser;

	TextReader *reader;
	int			status;
	cmdmap		cmd_table;
	labelmap	label_table;
};

// inline 成员函数

inline bool CScriptAction::IsSaveLoadOK() const
{
	return PlayMode != MODE_SYSTEM && (status == WaitKeyPressed || status == WaitMenuDone);
};

#endif
