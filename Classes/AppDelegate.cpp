#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Scene_main.h"
#include "cd_visibleRect.h"
#include "cd_preload.h"
#include "sceneManage.h"
#include "config.h"
#include "TH_log.h"
USING_NS_CC;

AppDelegate::AppDelegate() : m_sceneManage(nullptr) {

}

AppDelegate::~AppDelegate() 
{
	CC_SAFE_DELETE(m_sceneManage);
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	auto size = Size(WindowWidth, WindowHeight);
    if(!glview) {
		glview = GLViewImpl::createWithRect(ApplicationTitle, Rect(0, 0, size.width, size.height));
        director->setOpenGLView(glview);
    }
	
	director->getOpenGLView()->setDesignResolutionSize(size.width, size.height, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	

	// init
	//�����ʼ��
	CVisibleRect::init(size.width, size.height);

	

    FileUtils::getInstance()->addSearchPath("res");

	// ����������
	m_sceneManage = new CSceneManage();
	m_sceneManage->init();
	//Ԥ���س�ʼ��
	CPreload::createInstance();
	//��־
#ifdef COCOS2D_DEBUG
	auto logPath = FileUtils::getInstance()->fullPathForFilename("gameLog.txt");//FileUtils::getInstance()->getWritablePath()+"gameLog.txt";
	TH::log_Create(logPath.c_str(), LOG_ID_GAME);

#endif
	THLog_Write(LOG_ID_GAME, true, "Scene_main��������ʧ��!");
    // create a scene. it's an autorelease object
  //  auto scene = HelloWorld::createScene();
	//auto scene = CScene_main::createScene();
	auto scene = m_sceneManage->createScene(Scene_loading);//!bLogoTo ? Scene_logo :
	if (scene) director->runWithScene(scene);
	else
	{
		THLog_Write(LOG_ID_GAME, true, "Scene_main��������ʧ��!");
		return false;
	}
    // run
   // director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
