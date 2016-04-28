#include "Effect.h"
#include "Scene_main.h"


USING_NS_CC;
void CViewEffect::callbackC()
{
	m_EffectCnt = 2;
}
//
// ��Ч�����ļ̳�
// 
// һ�δ���һ�����趯�����ڵȴ�ʱ������
//
// ����ķֽ⶯��
//
bool EffectWipeIn::Step()
{
	//������Ч
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
// �����ķֽ⶯��
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
// ����ķֽ⶯��
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
// �����ķֽ⶯��
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