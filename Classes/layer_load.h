#ifndef __LAYER_LOAD_H__
#define __LAYER_LOAD_H__
// by cgq 2015-7-6

#include "cocos2d.h"

enum emLoadAni
{
	LoadAni_Load,		// 加载动画
	LoadAni_YunIn,		// 云关闭
	LoadAni_YunOut,		// 云打开
	LoadAni_YunInLoad,	// 云关闭 load
	LoadAni_YunOutLoad,	// 云上面加 load
};

class CLayer_load : public cocos2d::Layer
{
public:
	CREATE_FUNC(CLayer_load);

	// 创建load
	static CLayer_load* pushLoad(cocos2d::Node *node, int localZOrder, emLoadAni aniid);
	// un mask
	static void popLoad(cocos2d::Node *node, int localZOrder);

	// 初始化
    virtual bool init(void);
	// 进入
	virtual void onEnter(void);
	// 释放资源
	virtual void onExit(void);

public:
	// 播放
	void playAni(emLoadAni aniid);
	// 继续
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
