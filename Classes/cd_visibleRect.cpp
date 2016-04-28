#include "cd_visibleRect.h"

USING_NS_CC;

Rect CVisibleRect::s_visibleRectOd;
Rect CVisibleRect::s_visibleRect;
cocos2d::Size CVisibleRect::s_originaSize;

void CVisibleRect::init( float originalWidth, float originalHeight )
{
	s_originaSize.setSize(originalWidth, originalHeight);
	s_visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	s_visibleRectOd = s_visibleRect;
}

Rect CVisibleRect::getVisibleRect()
{
    return s_visibleRect;
}

cocos2d::Size CVisibleRect::getOriginaSize( void )
{
	return s_originaSize;
}

Vec2 CVisibleRect::left()
{
    return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Vec2 CVisibleRect::right()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Vec2 CVisibleRect::top()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Vec2 CVisibleRect::bottom()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
}

Vec2 CVisibleRect::center()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Vec2 CVisibleRect::leftTop()
{
    return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Vec2 CVisibleRect::rightTop()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Vec2 CVisibleRect::leftBottom()
{
    return s_visibleRect.origin;
}

Vec2 CVisibleRect::rightBottom()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
}

ResolutionPolicy CVisibleRect::getResolutionPolicy( const cocos2d::Size& size )
{
	Size _screenSize = s_visibleRect.size;
	if (_screenSize.width > size.width) return ResolutionPolicy::FIXED_WIDTH;
	else if (_screenSize.height > size.height) return ResolutionPolicy::FIXED_HEIGHT;
	else if (_screenSize.width > _screenSize.height) return ResolutionPolicy::FIXED_WIDTH;
	
	return ResolutionPolicy::FIXED_HEIGHT;
}

sResolution CVisibleRect::getDesignResolutionInfo( ResolutionPolicy _resolutionPolicy )
{
	Size _screenSize = s_visibleRect.size;
	Size _designResolutionSize = s_originaSize;
	float _scaleX = (float)_screenSize.width / _designResolutionSize.width;
	float _scaleY = (float)_screenSize.height / _designResolutionSize.height;

	sResolution ret;
	// 窗口缩放
	ret.fWinScaleX = _scaleX;
	ret.fWinScaleY = _scaleY;

	// ui缩放
	ret.fUIScaleX = MIN(_scaleX, _scaleY);
	ret.fUIScaleY = MIN(_scaleX, _scaleY);

	ret.uiOffset.x = fabs(_designResolutionSize.width*ret.fUIScaleX-_screenSize.width);
	ret.uiOffset.y = fabs(_designResolutionSize.height*ret.fUIScaleY-_screenSize.height);

// 	ret.uiOffset.x = fabs(_designResolutionSize.width - _designResolutionSize.width*ret.fUIScaleX);
// 	ret.uiOffset.y = fabs(_designResolutionSize.height - _designResolutionSize.height*ret.fUIScaleY);


	// 背景缩放
	float _scaleXbg = 1.0f;
	float _scaleYbg = 1.0f;
	if (_resolutionPolicy == ResolutionPolicy::NO_BORDER)
	{
		_scaleX = _scaleY = MAX(_scaleX, _scaleY);
	}
	else if (_resolutionPolicy == ResolutionPolicy::SHOW_ALL)
	{
		_scaleX = _scaleY = MIN(_scaleX, _scaleY);
	}
	else if (_resolutionPolicy == ResolutionPolicy::FIXED_HEIGHT)
	{
		_scaleX = _scaleY;
		_designResolutionSize.width = ceilf(_screenSize.width/_scaleX);
	}
	else if (_resolutionPolicy == ResolutionPolicy::FIXED_WIDTH) 
	{
		_scaleY = _scaleX;
		_designResolutionSize.height = ceilf(_screenSize.height/_scaleY);
	}

	ret.fBGScaleX = _scaleX;
	ret.fBGScaleY = _scaleY;

	return ret;
}

sResolution CVisibleRect::getDesignResolutionInfo( ResolutionPolicy _resolutionPolicy, const cocos2d::Size& size )
{
	auto oldsize = s_originaSize;
	s_originaSize = size;
	auto ret = getDesignResolutionInfo(_resolutionPolicy);
	s_originaSize = oldsize;
	return ret;
}

void CVisibleRect::beginRect( cocos2d::Size si )
{
	s_visibleRect.origin = Vec2::ZERO;
	s_visibleRect.size = si;
}

void CVisibleRect::endRect( void )
{
	s_visibleRect = s_visibleRectOd;
}

cocos2d::Vec2 CVisibleRect::screenAdaptationPos( sResolution &rl, cocos2d::Node* node )
{
	auto pos = node->getPosition();
	float _scaleX = s_visibleRect.size.width / (s_originaSize.width);
	float _scaleY = s_visibleRect.size.height / (s_originaSize.height);
 	if (fabs(_scaleX - _scaleY) < 0.001f)
 	{
 		return pos;
 	}
 //	_scaleX = rl.fBGScaleX;
 //	_scaleY = rl.fBGScaleY;

	// pos
	auto size = node->getContentSize();
	auto retpos = pos;

	// x
	float percent = (pos.x/(s_originaSize.width*rl.fUIScaleX));
	float offset = percent*s_visibleRect.size.width;
	float offset_of = MAX((size.width*_scaleX-size.width)/2, 0.0f);
// 	if (int(pos.x) != 0)
// 	{
// 		if (pos.x < s_originaSize.width/2.0f)
// 		{
// 			retpos.x = offset;
// 		}
// 		else if (pos.x > s_originaSize.width/2.0f)
// 		{
// 			retpos.x = offset + offset_of/2;
// 		}
// 		else
// 		{
// 			retpos.x = offset;// + s_visibleRect.size.width/2.0f/*+size.width*_scaleX/2.0f*/;
// 		}
// 	}
// 	else
	{
		retpos.x = offset;
	}
	
	// y
	percent = (pos.y/(s_originaSize.height*rl.fUIScaleY));
	offset = percent*s_visibleRect.size.height;
	offset_of = MAX((size.height*_scaleY-size.height)/2, 0.0f);
// 	if (int(pos.y) != 0)
// 	{
// 		if (pos.y < s_originaSize.height/2.0f)
// 		{
// 			retpos.y = offset - offset_of/2;
// 		}
// 		else if (pos.y > s_originaSize.height/2.0f)
// 		{
// 			retpos.y = offset + offset_of/2;
// 		}
// 		else
// 		{
// 			retpos.y = offset;// + s_visibleRect.size.width/2.0f/*+size.width*_scaleX/2.0f*/;
// 		}
// 	}
// 	else
	{
		retpos.y = offset;
	}
	
	return retpos;
}

void CVisibleRect::screenAdaptationNodechild( sResolution &rl, cocos2d::Node* node )
{
	auto &child = node->getChildren();
	for (auto &v: child)
	{
		v->setPosition(screenAdaptationPos(rl, v));
	}
}

cocos2d::Size CVisibleRect::getScaleToSize( const cocos2d::Size &wh )
{
	float _scaleX = s_visibleRect.size.width / wh.width;
	float _scaleY = s_visibleRect.size.height / wh.height;

	return Size(_scaleX, _scaleY);
}