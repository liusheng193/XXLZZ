//
// ������Ч
//
//
#ifndef	__effect_h__
#define	__effect_h__
#include "cocos2d.h"
#include "Config.h"
class CScene_main;

//
// ��Ч���
//
class CViewEffect 
{
public:
	CViewEffect(CScene_main *sence, float step, std::string src = "", const cocos2d::Rect &rect = cocos2d::Rect(0, 0, WindowWidth, WindowHeight));
	bool Step(float time);
	//�ص�
	void callbackC();
protected:
	virtual bool Step() = 0;
protected:
	CScene_main *m_Csmain;
	//ͼƬ����
	std::string		m_Src;		
	float			m_TimeBase;
	//��Χ
	cocos2d::Rect	m_EffectRect;
	//������
	int			m_EffectCnt;
};

// inline��Ա��ʽ

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
// ��Ч
//
// �̳�CViewEffect
//
// step����Ա����TimeBase������msΪ��λ��ָ������ģ�
// ����1��8Ӱ��Frame)��ʱ���ָ��Ϊ1000/8
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