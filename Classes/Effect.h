//
// 画面特效
//
//
#ifndef	__effect_h__
#define	__effect_h__
#include "cocos2d.h"
#include "Config.h"
class CScene_main;

//
// 特效类别
//
class CViewEffect 
{
public:
	CViewEffect(CScene_main *sence, float step, std::string src = "", const cocos2d::Rect &rect = cocos2d::Rect(0, 0, WindowWidth, WindowHeight));
	bool Step(float time);
	//回调
	void callbackC();
protected:
	virtual bool Step() = 0;
protected:
	CScene_main *m_Csmain;
	//图片名称
	std::string		m_Src;		
	float			m_TimeBase;
	//范围
	cocos2d::Rect	m_EffectRect;
	//计数器
	int			m_EffectCnt;
};

// inline成员函式

inline CViewEffect::CViewEffect(CScene_main *win, float step, std::string src, const cocos2d::Rect &rect)
:m_Csmain(win), m_Src(src), m_TimeBase(step), m_EffectRect(rect), m_EffectCnt(0)
{
}

inline bool CViewEffect::Step(float time)
{
	if (m_EffectCnt == 0)
		return Step();
	else if (m_EffectCnt == 2)
		return false;
	else 
		return true;
}


//
// 特效
//
// 继承CViewEffect
//
// step（成员变量TimeBase）是以ms为单位来指定间隔的，
// 所以1秒8影格（Frame)的时候就指定为1000/8
//

//
// Wipe in
//
class EffectWipeIn : public CViewEffect {
public:
	EffectWipeIn(CScene_main *win, std::string src, const  cocos2d::Rect &rect)
		:CViewEffect(win, 2.5, src, rect) {}

protected:
	bool Step();
};

//
// Wipe out
//
class EffectWipeOut : public CViewEffect {
public:
	EffectWipeOut(CScene_main *win, std::string src)
		:CViewEffect(win, 2.5, src) {}

protected:
	bool Step();
};

// Fade in
class EffectFadeIn : public CViewEffect {
public:
	EffectFadeIn(CScene_main *win, std::string src)
		:CViewEffect(win, 1.5, src) {}

protected:
	bool Step();
};

// Fade out
class EffectFadeOut : public CViewEffect {
public:
	EffectFadeOut(CScene_main *win, std::string src)
		:CViewEffect(win, 1.5, src) {}

protected:
	bool Step();
};

#endif