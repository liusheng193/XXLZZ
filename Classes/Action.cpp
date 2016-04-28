//
//  使用者的动作（基本）
//
//		Copyright (c) 2000-2001 Chihiro.SAKAMOTO (HyperWorks)
//
#include "Action.h"
#include "Scene_main.h"
//
// 析构函数(Destructor)
//
CAction::~CAction()
{
}

//
// 动作暂停
//
void CAction::Pause()
{
}

//
// 动作再开始
//
void CAction::Resume()
{
}

//
// 鼠标左键按下时的处理
//
void CAction::LButtonDown(UINT, cocos2d::Vec2)
{
}

//
// 鼠标左键放开时的处理
//
void CAction::LButtonUp(UINT, cocos2d::Vec2)
{
}

//
// 鼠标右键按下时的处理
//
void CAction::RButtonDown(UINT, cocos2d::Vec2)
{
}

//
// 鼠标右键放开时的处理
//
void CAction::RButtonUp(UINT, cocos2d::Vec2)
{
}

//
// 鼠标值标移动时的处理
//
void CAction::MouseMove(UINT, cocos2d::Vec2)
{
}

//
// 键盘按下时的处理
//
void CAction::KeyDown(UINT)
{
}

//
// IDLE的处理
//
bool CAction::IdleAction()
{
	return false;
}

//
// 计时器（timer）的处理
//
bool CAction::TimedOut(int timerId)
{
	return true;
}

//
// 音乐播放结束时的处理
//
// ·同一首再次播放
//
void CAction::MusicDone(int music)
{
	if (music)
		Parent->RestartMusic();
}

//
// 擦入结束时的处理
//
void CAction::WipeDone()
{
}

//
// Wave播放结束时的处理
//
void CAction::WaveDone()
{
}

//
// 是否可储存/载入?
//
bool CAction::IsSaveLoadOK() const
{
	return false;	// 预设为不可
}

//
// 中断
//
void CAction::Abort()
{
}
void CAction::SetMenuSelect(int sel)
{
}