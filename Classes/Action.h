//
// ʹ���ߵĶ���(����)
//
//		Copyright (c) 2000-2001 Chihiro.SAKAMOTO (HyperWorks)
//
#ifndef	__action_h__
#define	__action_h__
#include "cocos2d.h"
typedef unsigned int        UINT;
//
//	�������
//
//	�Ѵ����粻���ڷ������¼��ȣ���
//	�Ӹ���״����д�����CMainWin���ó���
//

class CScene_main;

class CAction {
  public:
	  CAction(CScene_main *win, CAction *oldAction = 0) : Parent(win), OldAction(oldAction) {}
	virtual ~CAction();

	virtual	void Pause();
	virtual	void Resume();

	virtual	void LButtonDown(UINT modKeys, cocos2d::Vec2 point);
	virtual	void LButtonUp(UINT modKeys, cocos2d::Vec2  point);
	virtual	void RButtonDown(UINT modKeys, cocos2d::Vec2  point);
	virtual	void RButtonUp(UINT modKeys, cocos2d::Vec2  point);
	virtual	void MouseMove(UINT modKeys, cocos2d::Vec2  point);
	virtual	void KeyDown(UINT key);
	virtual bool TimedOut(int timerId);
	virtual bool IdleAction();
	virtual void MusicDone(int music);
	virtual void WipeDone();
	virtual void WaveDone();
	virtual bool IsSaveLoadOK() const;
	virtual void Abort();
	virtual void SetMenuSelect(int sel);
	CAction *GetOldAction() const { return OldAction; }

  protected:
	CAction    *OldAction;
	CScene_main	*Parent;
} ;

#endif
