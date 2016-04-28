//
//  ʹ���ߵĶ�����������
//
//		Copyright (c) 2000-2001 Chihiro.SAKAMOTO (HyperWorks)
//
#include "Action.h"
#include "Scene_main.h"
//
// ��������(Destructor)
//
CAction::~CAction()
{
}

//
// ������ͣ
//
void CAction::Pause()
{
}

//
// �����ٿ�ʼ
//
void CAction::Resume()
{
}

//
// ����������ʱ�Ĵ���
//
void CAction::LButtonDown(UINT, cocos2d::Vec2)
{
}

//
// �������ſ�ʱ�Ĵ���
//
void CAction::LButtonUp(UINT, cocos2d::Vec2)
{
}

//
// ����Ҽ�����ʱ�Ĵ���
//
void CAction::RButtonDown(UINT, cocos2d::Vec2)
{
}

//
// ����Ҽ��ſ�ʱ�Ĵ���
//
void CAction::RButtonUp(UINT, cocos2d::Vec2)
{
}

//
// ���ֵ���ƶ�ʱ�Ĵ���
//
void CAction::MouseMove(UINT, cocos2d::Vec2)
{
}

//
// ���̰���ʱ�Ĵ���
//
void CAction::KeyDown(UINT)
{
}

//
// IDLE�Ĵ���
//
bool CAction::IdleAction()
{
	return false;
}

//
// ��ʱ����timer���Ĵ���
//
bool CAction::TimedOut(int timerId)
{
	return true;
}

//
// ���ֲ��Ž���ʱ�Ĵ���
//
// ��ͬһ���ٴβ���
//
void CAction::MusicDone(int music)
{
	if (music)
		Parent->RestartMusic();
}

//
// �������ʱ�Ĵ���
//
void CAction::WipeDone()
{
}

//
// Wave���Ž���ʱ�Ĵ���
//
void CAction::WaveDone()
{
}

//
// �Ƿ�ɴ���/����?
//
bool CAction::IsSaveLoadOK() const
{
	return false;	// Ԥ��Ϊ����
}

//
// �ж�
//
void CAction::Abort()
{
}
void CAction::SetMenuSelect(int sel)
{
}