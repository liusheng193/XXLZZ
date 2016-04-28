#ifndef __LAYER_MASK_H__
#define __LAYER_MASK_H__
// by cgq 2015-1-10

#include "cocos2d.h"

class CLayer_mask : public cocos2d::Layer
{
public:
	CREATE_FUNC(CLayer_mask);

	// 创建mask
	static CLayer_mask* pushMask(cocos2d::Node *node, int localZOrder);
	// un mask
	static void popMask(cocos2d::Node *node, int localZOrder);

	// 初始化
    virtual bool init(void);
	// 进入
	virtual void onEnter(void);
	// 释放资源
	virtual void onExit(void);

	// msg
public:
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event){}
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event){}

protected:
	cocos2d::Node*	m_nodeRoot;
};

#endif // __LAYER_MASK_H__
