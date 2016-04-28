#ifndef __CD_VISIBLERECT_H__
#define __CD_VISIBLERECT_H__
// by cgq 2015-1-10

#include "cocos2d.h"

struct sResolution
{
	float fUIScaleX;		// ui控件缩放
	float fUIScaleY;		// ui控件缩放

	float fBGScaleX;		// 背景图缩放
	float fBGScaleY;		// 背景图缩放

	float fWinScaleX;		// 缩放到窗口
	float fWinScaleY;		// 缩放到窗口

	cocos2d::Vec2 uiOffset;	// ui位置偏移
};


class CVisibleRect
{
public:
	static void init(float originalWidth, float originalHeight);

    static cocos2d::Rect getVisibleRect(void);
	static cocos2d::Size getOriginaSize(void);

	static void beginRect(cocos2d::Size si);
	static void endRect(void);

    static cocos2d::Vec2 left(void);
    static cocos2d::Vec2 right(void);
    static cocos2d::Vec2 top(void);
    static cocos2d::Vec2 bottom(void);
    static cocos2d::Vec2 center(void);
    static cocos2d::Vec2 leftTop(void);
    static cocos2d::Vec2 rightTop(void);
    static cocos2d::Vec2 leftBottom(void);
    static cocos2d::Vec2 rightBottom(void);

	// 适配
	static ResolutionPolicy getResolutionPolicy(const cocos2d::Size& size);

	// 获取最佳缩放率
	static sResolution getDesignResolutionInfo(ResolutionPolicy resolutionPolicy);
	static sResolution getDesignResolutionInfo(ResolutionPolicy resolutionPolicy, const cocos2d::Size& size);

	// 获取缩放到窗口
	static cocos2d::Size getScaleToSize(const cocos2d::Size &wh);

	// 获取适配位置
	static cocos2d::Vec2 screenAdaptationPos(sResolution &rl, cocos2d::Node* node);

	// NODE，子节点 适配
	static void screenAdaptationNodechild(sResolution &rl, cocos2d::Node* node);

private:
    static cocos2d::Rect s_visibleRectOd;
	static cocos2d::Rect s_visibleRect;
	static cocos2d::Size s_originaSize;
};

#endif /* __CD_VISIBLERECT_H__ */
