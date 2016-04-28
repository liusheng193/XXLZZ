#include "scene_loading.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


#include "cd_visibleRect.h"
#include "cd_preload.h"
#include "sceneManage.h"
#include "cfg_config.h"


USING_NS_CC;
using namespace cocostudio;


CScene_loading* CScene_loading::s_sceneLoading = nullptr;

CScene_loading* CScene_loading::getInstance(void)
{
	return s_sceneLoading;
}

void CScene_loading::loadingPercent(float pt)
{
	if (s_sceneLoading)
	{
		s_sceneLoading->addPercent(pt);
	}
}


bool CScene_loading::init()
{
	if (!Layer::init())
	{
		return false;
	}
	s_sceneLoading = this;
	m_clickEventListener = nullptr;
	m_bLoading = false;
	m_bLoadingOk = false;
	auto sprbg = Sprite::create("loadbg.png");
	sprbg->setVisible(false);
	this->addChild(sprbg);
	auto nd_bg = CSLoader::createNode(SCENCEPATH"main_loading.csb");
	this->addChild(nd_bg);
	//Sprite* spr = nullptr;
	//this->addChild(spr);
	m_nodeRoot = nd_bg;
	
	// 适配
	auto resolution = CVisibleRect::getDesignResolutionInfo(ResolutionPolicy::SHOW_ALL);
	nd_bg->setScale(resolution.fUIScaleX, resolution.fUIScaleY);
	CVisibleRect::screenAdaptationNodechild(resolution, nd_bg);

	auto scenesize = CVisibleRect::getVisibleRect().size;
	auto bgsize = sprbg->getContentSize();
	float _scaleX = (float)scenesize.width / bgsize.width;
	float _scaleY = (float)scenesize.height / bgsize.height;
	ResolutionPolicy rp = ResolutionPolicy::FIXED_HEIGHT;
	if (_scaleX > _scaleY) rp = ResolutionPolicy::FIXED_WIDTH;
	auto resolutionBg = CVisibleRect::getDesignResolutionInfo(rp, bgsize);
	sprbg->setScale(resolutionBg.fBGScaleX, resolutionBg.fBGScaleY);
	sprbg->setPosition(CVisibleRect::center());
	nd_bg->setPosition(CVisibleRect::center());
	// 加载资源
	_loading();
	setKeyboardEnabled(true);
	return true;
}
void CScene_loading::onEnter(void)
{
	Layer::onEnter();
}

void CScene_loading::onExit(void)
{
	Layer::onExit();
	s_sceneLoading = nullptr;

	//	thUtility::removeMessageBox(Messagebox_msg);
	this->stopAllActions();
	this->removeAllChildren();
	//CPreload::removeDat("scene/ui/loading/eny.dat");
	//Director::getInstance()->getTextureCache()->removeTextureForKey("scene/ui/loading/loading_bg.jpg");
}
void CScene_loading::setPercent(float pt)
{
	auto jd = (ui::LoadingBar*)m_nodeRoot->getChildByTag(10);
	jd->setPercent(pt);
	char szbuf[100];
	int per = (int)pt;
	snprintf(szbuf, 100, "%d %%", per);
	auto txt = dynamic_cast<ui::Text*>(m_nodeRoot->getChildByTag(8));
	txt->setString(szbuf);
}
void CScene_loading::addPercent(float pt)
{
	m_fLoadPercentAll += pt;
	setPercent(m_fLoadPercentAll);
}

void CScene_loading::popData(void)
{
	setPercent(100.0f);
}


void CScene_loading::_loading(void)
{
	// 开始加载资源
	if (!m_bLoading)
	{
		// 		m_nodeRoot->getChildByTag(6)->setVisible(false);
		// 		m_nodeRoot->getChildByTag(20)->setVisible(true);
		setPercent(0.0f);

		auto preload = CPreload::getInstance();

		//// 资源数据
		CPreload::preloadDatAsync(UIPATH"eny.dat", CC_CALLBACK_3(CScene_loading::_loadingResCallBack, this, LoadingType_ui), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBackMessage, this));
		//CPreload::preloadDatAsync("scene/ani/eny.dat", CC_CALLBACK_3(CScene_loading::_loadingResCallBack, this, LoadingType_ani), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBack, this));
		//CPreload::preloadDatAsync("scene/ani/wj/eny.dat", CC_CALLBACK_3(CScene_loading::_loadingResCallBack, this, LoadingType_ani_wujian), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBack, this));
		//CPreload::preloadDatAsync("scene/ui/map/eny.dat", CC_CALLBACK_3(CScene_loading::_loadingResCallBack, this, LoadingType_ui_map), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBack, this));
		//// CPreload::preloadDatAsync("scene/ui/map/wj/eny.dat", CC_CALLBACK_2(CScene_loading::_loadingResCallBack, this, LoadingType_ui_map_wj), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBack, this));
		//CPreload::preloadDatAsync("scene/ui/shop/eny.dat", CC_CALLBACK_3(CScene_loading::_loadingResCallBack, this, LoadingType_ui_shop), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBack, this));
		//CPreload::preloadDatAsync("scene/ui/battle/eny.dat", CC_CALLBACK_3(CScene_loading::_loadingResCallBack, this, LoadingType_ui_shop), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBack, this));

		// 战场UI 添加
		CPreload::preloadDatAsync(BATTLEPATH"eny.dat", CC_CALLBACK_3(CScene_loading::_loadingResCallBack, this, LoadingType_ui_battle), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBack, this));
		//CPreload::preloadDatAsync("battlefieldAI/Soldiers/eny.dat", CC_CALLBACK_3(CScene_loading::_loadingResCallBack, this, LoadingType_battlefieldAI_Soldiers), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBack, this));
		//CPreload::preloadDatAsync("battlefieldAI/UI/AimTarget/eny.dat", CC_CALLBACK_3(CScene_loading::_loadingResCallBack, this, LoadingType_ui_battle), CC_CALLBACK_1(CScene_loading::_loadingResEndCallBack, this));

		// 配置文件加载
		CPreload::preloadDatAsync(CONFIGPATH"eny.dat", CC_CALLBACK_3(CScene_loading::_loadingConfig, this, LoadingType_cfg_main), CC_CALLBACK_1(CScene_loading::_loadingConfigEndCallBack, this));
		//CPreload::preloadDatAsync("data/config/map/eny.dat", CC_CALLBACK_3(CScene_loading::_loadingConfig, this, LoadingType_cfg_main), CC_CALLBACK_1(CScene_loading::_loadingConfigEndCallBack, this));

		m_iLoadingCountMax = CPreload::getPreloadCount();
		if (m_iLoadingCountMax == 0)m_iLoadingCountMax = m_iLoadingCount;
		m_iLoadingCount = 1;


		m_fLoadPercentMax = 70.0f;
		m_fLoadPercentTexMax = m_fLoadPercentMax / m_iLoadingCountMax;
		m_fLoadPercentAll = m_fLoadPercentMax - m_fLoadPercentTexMax*m_iLoadingCountMax;
	}
	m_bLoading = true;
}

int CScene_loading::_loadingResCallBack(const std::string& name, const std::string& buf, int count, int type)
{
	MapLoadingVal::iterator iter = m_mapLoadingVal.find(type);
	if (iter == m_mapLoadingVal.end())
	{
		if (count != 0)
		{
			sLoadingVl val;
			val.fLoadPercentAll = 0.0f;
			val.fLoadPercentVal = m_fLoadPercentTexMax / count;
			m_mapLoadingVal.insert(MapLoadingVal::value_type(type, val));
			iter = m_mapLoadingVal.find(type);
		}
		return 0;
	}

	iter->second.fLoadPercentAll += iter->second.fLoadPercentVal;
	m_fLoadPercentAll += iter->second.fLoadPercentVal;
	setPercent(m_fLoadPercentAll);

	return 0;
}


void CScene_loading::_loadingResEndCallBack(int)
{
	// 加载完成
	m_iLoadingCount += 1;
	if (m_iLoadingCount > m_iLoadingCountMax)
	{
		//m_nodeRoot->getChildByTag(20)->setVisible(true);
		{
			m_mapLoadingVal.clear();
			setPercent(m_fLoadPercentAll);
			if (m_clickEventListener)
			{
				m_clickEventListener();
			}
			m_clickEventListener = nullptr;
		}
	}
}
void CScene_loading::_loadingResEndCallBackMessage(int)
{
	_loadingResEndCallBack(0);
}

int CScene_loading::_loadingConfig(const std::string& name, const std::string& buf, int count, int type)
{
	if (name == "map_bk.json")
	{
		//CCfg_config::getInstance()->loadConfig_map_bk(buf);
	}
	return 0;
}
void CScene_loading::_loadingConfigEndCallBack(int)
{
	m_fLoadPercentAll += m_fLoadPercentTexMax;
	setPercent(m_fLoadPercentAll);
	_loadingResEndCallBack(0);
}
void CScene_loading::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_E)
	{
	}
}
void CScene_loading::setStartGameMsg(const std::function<void(void)>& callback)
{
	m_clickEventListener = callback;
}
