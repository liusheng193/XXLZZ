#include "Effect.h"
#include "Scene_main.h"


USING_NS_CC;
void CViewEffect::callbackC()
{
	m_EffectCnt = 2;
}
//
// 特效动作的继承
// 
// 一次处理一个步骤动作，在等待时可输入
//
// 擦入的分解动作
//
bool EffectWipeIn::Step()
{
	//擦除特效
	//auto t1=PageTurn3D::create(m_TimeBase, Size(15, 10));
	auto t1 = FadeOutUpTiles::create(m_TimeBase, Size(32, 24));
	//auto back = fadeout->reverse();
	auto t2 = t1->reverse();
	auto funcall = cocos2d::CallFunc::create(CC_CALLBACK_0(CViewEffect::callbackC, this));
	auto  sq = Sequence::create(t2, funcall, nullptr);
	m_Csmain->EffectRunAction(sq);
	m_EffectCnt = 1;
	

	return true;
}
//
// 擦出的分解动作
//
bool EffectWipeOut::Step()
{
	auto t1 = FadeOutUpTiles::create(m_TimeBase, Size(32, 24));
	auto funcall = cocos2d::CallFunc::create(CC_CALLBACK_0(CViewEffect::callbackC, this));
	auto  sq = Sequence::create(t1, funcall, nullptr);
	m_Csmain->EffectRunAction(sq);
	m_EffectCnt = 1;

	return true;
}

//
// 淡入的分解动作
//
bool EffectFadeIn::Step()
{
	auto t1 = FadeIn::create(m_TimeBase);
	//auto t1 = FadeOutUpTiles::create(m_TimeBase, Size(32, 24));
	//auto t2 = t1->reverse();
	auto funcall = cocos2d::CallFunc::create(CC_CALLBACK_0(CViewEffect::callbackC, this));
	auto  sq = Sequence::create(t1, funcall, nullptr);
	m_Csmain->EffectRunAction(sq);
	m_EffectCnt = 1;
	return true;
}
//
// 淡出的分解动作
//
bool EffectFadeOut::Step()
{
	auto t1 = FadeOut::create(m_TimeBase);
	//auto t1 = FadeOutUpTiles::create(m_TimeBase, Size(32, 24));
	//auto t2 = t1->reverse();
	auto funcall = cocos2d::CallFunc::create(CC_CALLBACK_0(CViewEffect::callbackC, this));
	auto  sq = Sequence::create(t1, funcall, nullptr);
	m_Csmain->EffectRunAction(sq);
	m_EffectCnt = 1;
	return true;
}