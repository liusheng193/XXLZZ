#include "layer_mask.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "AppDelegate.h"
#include "cd_visibleRect.h"

#define LAYER_MASK	9999

USING_NS_CC;
using namespace cocostudio;

CLayer_mask* CLayer_mask::pushMask( cocos2d::Node *node, int localZOrder )
{
	auto nd = node->getChildByTag(localZOrder);
	if (nd != nullptr)
	{
		return dynamic_cast<CLayer_mask*>(nd);
 	}
	popMask(node, localZOrder);

	auto mask = CLayer_mask::create();
	mask->setTag(localZOrder);
	node->addChild(mask, localZOrder);

	return mask;
}

void CLayer_mask::popMask( cocos2d::Node *node, int localZOrder )
{
	node->removeChildByTag(localZOrder);
}

bool CLayer_mask::init()
{
    if (!Layer::init())
    {
        return false;
    }
	auto oSize = CVisibleRect::getOriginaSize();
	
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	this->setSwallowsTouches(true);
	this->setTouchEnabled(true);

	auto sprTm = Sprite::create();
	sprTm->setColor(Color3B(0,0,0));
	sprTm->setTextureRect(Rect(0,0,oSize.width,oSize.height));
	sprTm->setOpacity(128);
	this->addChild(sprTm);

	// ÊÊÅä
	auto resolution = CVisibleRect::getDesignResolutionInfo(ResolutionPolicy::FIXED_HEIGHT);
	sprTm->setScale(resolution.fWinScaleX, resolution.fWinScaleY);
	sprTm->setPosition(CVisibleRect::center());

    return true;
}

void CLayer_mask::onEnter( void )
{
	Layer::onEnter();
}

void CLayer_mask::onExit( void )
{
	Layer::onExit();
	this->stopAllActions();
	this->removeAllChildren();
}

bool CLayer_mask::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *unused_event )
{
	return true;
}

void CLayer_mask::onTouchEnded( cocos2d::Touch *touch, cocos2d::Event *unused_event )
{
}
