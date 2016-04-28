#include "sceneManage.h"



#include "SimpleAudioEngine.h"
#include "cd_preload.h"
#include "Scene_main.h"
#include "scene_loading.h"
#include "utility.h"
#include "cfg_config.h"


USING_NS_CC;
using namespace CocosDenshion;


CSceneManage* CSceneManage::s_sceneManage = nullptr;
CSceneManage* CSceneManage::getInstance(void)
{
	return s_sceneManage;
}

CSceneManage::~CSceneManage(void)
{
	s_sceneManage = nullptr;
}

bool CSceneManage::init(void)
{
	if (!Node::init())
	{
		return false;
	}
	m_bExit = false;
	s_sceneManage = this;
	m_curLayer = nullptr;
	m_emSceneType = Scene_note;
	m_loadScene = nullptr;

	CCfg_config::getInstance();
	//配置
	thUtility::getInstance()->loadStringFile(XMLPATH"string.xml");
	/*thUtility::getInstance()->loadStringFile("data/string.xml");
	thUtility::getInstance()->loadStringFile("data/string_network.xml");
	thUtility::getInstance()->loadStringFile("data/stringinfo.xml");

	Director::getInstance()->setEidtCallback(CC_CALLBACK_0(CSceneManage::_editGame, this));*/
	return true;
}

void CSceneManage::_editGame(void)
{
	if (m_bExit) return;
	m_bExit = true;
	auto main = CScene_main::getInstance();
	if (main) main->release();
	CPreload::destroyInstance();
}
void CSceneManage::ingame(void)
{
	CPreload::preloadEnd();
	if (m_loadScene)
	{
		m_loadScene->popData();
		m_loadScene = nullptr;
	}
}


cocos2d::Scene* CSceneManage::createScene(emSceneType type)
{
	switch (type)
	{
	case Scene_logo:			// logo
	{
									//auto logo = CScene_logo::create();
								//	m_curLayer = logo;
									//break;
	}
	case Scene_loading:			// 加载界面场景
	{
									m_loadScene = CScene_loading::create();
									m_loadScene->setStartGameMsg(CC_CALLBACK_0(CSceneManage::_startGame, this));
									m_curLayer = m_loadScene;
									break;
	}
	case Scene_regame:			// 重新加载游戏
	{
								//	auto regame = CScene_regame::create();
								//	m_curLayer = regame;
								//	break;
	}
	case Scene_main:			// 主界面场景
	{
									auto sceneMain = CScene_main::create();
									m_curLayer = sceneMain;
									break;
	}
	default:
	{
			   if (m_curLayer)
			   {
				   m_curLayer->release();
				   m_curLayer = nullptr;
			   }
			   return nullptr;
	}
	}
	if (!m_curLayer)
	{
		return nullptr;
	}

	m_emSceneType = type;
	m_curScene = Scene::create();
	m_curScene->addChild(m_curLayer);
	return m_curScene;
}
void CSceneManage::_checkGame(void)
{

}
void CSceneManage::_startGame(void)
{
	_loadResource();
	ingame();
	auto scene_menu = createScene(Scene_main);
	if (!scene_menu) return;

	// 切换场景
	auto tsScene = TransitionFade::create(1.0f, m_curScene);
	Director::getInstance()->replaceScene(tsScene);
}
void CSceneManage::_loadResource(void)
{
	// 加载资源

	// 读取配置
}