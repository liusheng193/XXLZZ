#ifndef __LAYER_LOAD_H__
#define __LAYER_LOAD_H__
// by cgq 2015-7-6

#include "cocos2d.h"

enum emLoadAni
{
	LoadAni_Load,		// ���ض���
	LoadAni_YunIn,		// �ƹر�
	LoadAni_YunOut,		// �ƴ�
	LoadAni_YunInLoad,	// �ƹر� load
	LoadAni_YunOutLoad,	// ������� load
};

class CLayer_load : public cocos2d::Layer
{
public:
	CREATE_FUNC(CLayer_load);

	// ����load
	static CLayer_load* pushLoad(cocos2d::Node *node, int localZOrder, emLoadAni aniid);
	// un mask
	static void popLoad(cocos2d::Node *node, int localZOrder);

	// ��ʼ��
    virtual bool init(void);
	// ����
	virtual void onEnter(void);
	// �ͷ���Դ
	virtual void onExit(void);

public:
	// ����
	void playAni(emLoadAni aniid);
	// ����
	void resumeAni(emLoadAni aniid, const std::function<void (void)>& callfunc);

	// msg
public:
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event){}
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event){}

protected:
	cocos2d::Node*	m_nodeRoot;
};

#endif // __LAYER_LOAD_H__
