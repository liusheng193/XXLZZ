#include "layer_load.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "AppDelegate.h"
#include "cd_visibleRect.h"

USING_NS_CC;
using namespace cocostudio;

CLayer_load* CLayer_load::pushLoad( cocos2d::Node *node, int localZOrder, emLoadAni aniid )
{
	auto nd = node->getChildByTag(localZOrder);
	if (nd != nullptr)
	{
		return dynamic_cast<CLayer_load*>(nd);
 	}
	popLoad(node, localZOrder);

	auto mask = CLayer_load::create();
	mask->setTag(localZOrder);
	node->addChild(mask, localZOrder);
	mask->playAni(aniid);

	return mask;
}

void CLayer_load::popLoad( cocos2d::Node *node, int localZOrder )
{
	node->removeChildByTag(localZOrder);
}

bool CLayer_load::init()
{
    if (!Layer::init())
    {
        return false;
    }
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	this->setSwallowsTouches(true);
	this->setTouchEnabled(true);

	auto oSize = CVisibleRect::getOriginaSize();
	auto sprTm = Sprite::create();
	sprTm->setTag(200);
	sprTm->setColor(Color3B(0,0,0));
	sprTm->setTextureRect(Rect(0,0,oSize.width,oSize.height));
	sprTm->setOpacity(30);
	this->addChild(sprTm);

	// ÔÆ¶¯»­
	ArmatureDataManager::getInstance()->addArmatureFileInfo("scene/ui/cloudAnimation.ExportJson");
	auto ani1 = Armature::create("cloudAnimation");
	ani1->setTag(301);
	ani1->setVisible(false);
	float fs = ani1->getAnimation()->getSpeedScale();
	ani1->getAnimation()->setSpeedScale(fs*4.0f);
	this->addChild(ani1);

	// load
	ArmatureDataManager::getInstance()->addArmatureFileInfo("scene/ui/ani_load.csb");
	auto ani = Armature::create("ani_load");
	ani->setTag(300);
	ani->setAnchorPoint(Vec2(0.5f,0.5f));
	ani->setVisible(false);
	this->addChild(ani);

	// ÊÊÅä
	auto size = ani1->getContentSize();
	auto rp = CVisibleRect::getResolutionPolicy(size);
	auto resolution = CVisibleRect::getDesignResolutionInfo(rp);
	ani1->setScale(resolution.fBGScaleX, resolution.fBGScaleY);
	ani1->setPosition(CVisibleRect::center());

	ani->setScale(resolution.fUIScaleX, resolution.fUIScaleY);
	ani->setPosition(CVisibleRect::center());

	sprTm->setScale(resolution.fWinScaleX, resolution.fWinScaleY);
	sprTm->setPosition(CVisibleRect::center());

    return true;
}

void CLayer_load::onEnter( void )
{
	Layer::onEnter();
}

void CLayer_load::onExit( void )
{
	Layer::onExit();
	this->stopAllActions();
	this->removeAllChildren();
}

bool CLayer_load::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *unused_event )
{
	return true;
}

void CLayer_load::onTouchEnded( cocos2d::Touch *touch, cocos2d::Event *unused_event )
{
}

void CLayer_load::playAni( emLoadAni aniid )
{
	auto nd1 = getChildByTag(300); nd1->setVisible(false);
	auto nd2 = getChildByTag(301); nd2->setVisible(false);
	switch (aniid)
	{
	case LoadAni_Load:
		{
			nd1->setVisible(true);
			auto ani = dynamic_cast<Armature*>(nd1);
			ani->getAnimation()->setMovementEventCallFunc(nullptr);
			ani->getAnimation()->play("play", true);
		}
		break;
	case LoadAni_YunIn:
		{
			nd2->setVisible(true);
			auto ani = dynamic_cast<Armature*>(nd2);
			ani->getAnimation()->setMovementEventCallFunc(nullptr);
			ani->getAnimation()->play("zoomIn", false);
		}
		break;
	case LoadAni_YunOut:
		{
			nd2->setVisible(true);
			auto ani = dynamic_cast<Armature*>(nd2);
			ani->getAnimation()->setMovementEventCallFunc(nullptr);
			ani->getAnimation()->play("zoomOut", false);
			ani->getAnimation()->pause();
		}
		break;
	case LoadAni_YunInLoad:
		{
			nd1->setVisible(true);
			auto ani1 = dynamic_cast<Armature*>(nd1);
			ani1->getAnimation()->setMovementEventCallFunc(nullptr);
			ani1->getAnimation()->play("play", true);

			nd2->setVisible(true);
			auto ani2 = dynamic_cast<Armature*>(nd2);
			ani2->getAnimation()->setMovementEventCallFunc(nullptr);
			ani2->getAnimation()->play("zoomIn", false);
		}
		break;
	case LoadAni_YunOutLoad:
		{
			nd1->setVisible(true);
			auto ani1 = dynamic_cast<Armature*>(nd1);
			ani1->getAnimation()->setMovementEventCallFunc(nullptr);
			ani1->getAnimation()->play("play", true);

			nd2->setVisible(true);
			auto ani2 = dynamic_cast<Armature*>(nd2);
			ani2->getAnimation()->setMovementEventCallFunc(nullptr);
			ani2->getAnimation()->play("zoomOut", false);
			ani2->getAnimation()->pause();
		}
		break;
	}
}

void CLayer_load::resumeAni( emLoadAni aniid, const std::function<void (void)>& callfunc )
{
	std::function<void(Armature *armature, MovementEventType movementType, const std::string& movementID)> frameCallfunc = [=](Armature *armature, MovementEventType movementType, const std::string& movementID)
	{
		if (movementType == COMPLETE)
		{
			callfunc();
		}
	};

	auto nd1 = getChildByTag(300);
	auto nd2 = getChildByTag(301);
	switch (aniid)
	{
	case LoadAni_Load:
		break;
	case LoadAni_YunIn:
		break;
	case LoadAni_YunOut:
		{
			auto ani = dynamic_cast<Armature*>(nd2);
			if (ani)
			{
				ani->getAnimation()->setMovementEventCallFunc(frameCallfunc);
				ani->getAnimation()->resume();
			}
		}
		break;
	case LoadAni_YunOutLoad:
		{
			if (nd1) nd1->setVisible(false);

			auto ani = dynamic_cast<Armature*>(nd2);
			if (ani)
			{
				ani->getAnimation()->setMovementEventCallFunc(frameCallfunc);
				ani->getAnimation()->resume();
			}
		}
		break;
	}
}
