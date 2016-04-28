#ifndef __CD_VISIBLERECT_H__
#define __CD_VISIBLERECT_H__
// by cgq 2015-1-10

#include "cocos2d.h"

struct sResolution
{
	float fUIScaleX;		// ui�ؼ�����
	float fUIScaleY;		// ui�ؼ�����

	float fBGScaleX;		// ����ͼ����
	float fBGScaleY;		// ����ͼ����

	float fWinScaleX;		// ���ŵ�����
	float fWinScaleY;		// ���ŵ�����

	cocos2d::Vec2 uiOffset;	// uiλ��ƫ��
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

	// ����
	static ResolutionPolicy getResolutionPolicy(const cocos2d::Size& size);

	// ��ȡ���������
	static sResolution getDesignResolutionInfo(ResolutionPolicy resolutionPolicy);
	static sResolution getDesignResolutionInfo(ResolutionPolicy resolutionPolicy, const cocos2d::Size& size);

	// ��ȡ���ŵ�����
	static cocos2d::Size getScaleToSize(const cocos2d::Size &wh);

	// ��ȡ����λ��
	static cocos2d::Vec2 screenAdaptationPos(sResolution &rl, cocos2d::Node* node);

	// NODE���ӽڵ� ����
	static void screenAdaptationNodechild(sResolution &rl, cocos2d::Node* node);

private:
    static cocos2d::Rect s_visibleRectOd;
	static cocos2d::Rect s_visibleRect;
	static cocos2d::Size s_originaSize;
};

#endif /* __CD_VISIBLERECT_H__ */
