#ifndef __SCENE_MANAGE_H__
#define __SCENE_MANAGE_H__
//场景管理
#include "Config.h"

class CScene_loading;
class CSceneManage :public cocos2d::Node
{
public:
	~CSceneManage(void);
	CREATE_FUNC(CSceneManage);
	static CSceneManage* getInstance(void);

	// 初始化
	virtual bool init(void);

public:
	// 创建场景
	cocos2d::Scene* createScene(emSceneType type);
	void	ingame(void);
protected:
	// 游戏结束退出
	void _editGame(void);
	// 游戏检测
	void _checkGame(void);

	// 开始游戏
	void _startGame(void);

	// 加载资源
	void _loadResource(void);
protected:
	static CSceneManage* s_sceneManage;
	emSceneType			m_emSceneType;
	cocos2d::Layer*		m_curLayer;
	cocos2d::Scene*		m_curScene;
	OkCallFunc			m_callfunc_start;

	CScene_loading*		m_loadScene;			// 加载界面
	bool				m_bExit;				// 退出

};


#endif