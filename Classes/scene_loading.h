#ifndef __SCENE_LOADING_H__
#define __SCENE_LOADING_H__

#include "cocos2d.h"
#include "Config.h"
class CScene_loading : public cocos2d::Layer
{
public:
	CREATE_FUNC(CScene_loading);
	static CScene_loading* getInstance(void);
	static void loadingPercent(float pt);

	// 初始化
	virtual bool init(void);
	// 进入
	virtual void onEnter(void);
	// 释放资源
	virtual void onExit(void);

public:
	// 设置消息回调
	void setStartGameMsg(const std::function<void(void)>& callback);
	// 设置进度
	void setPercent(float pt);
	void addPercent(float pt);
	float getPercent(void){ return m_fLoadPercentAll; }
	// 是否加载ok
	bool isLoadingOk(void){ return m_bLoadingOk; }
	// 清理数据
	void popData(void);

	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
protected:
	// 加载资源
	void _loading(void);
	// 资源加载回调
	int _loadingResCallBack(const std::string& name, const std::string& buf, int count, int type);
	void _loadingResEndCallBack(int);
	void _loadingResEndCallBackMessage(int);

	// 配置文件加载
	int _loadingConfig(const std::string& name, const std::string& buf, int count, int type);
	void _loadingConfigEndCallBack(int);

protected:
	// msg
	void _btn_msg_CallBack_messagebox(int msgid, int btnid, const IntCallFunc& okcallback);

	void _btn_msg_CallBack_login(cocos2d::Ref* ref);

protected:
	std::function<void(void)> m_clickEventListener;

protected:
	static CScene_loading* s_sceneLoading;		// this
	cocos2d::Node*	m_nodeRoot;
	bool	m_bLoading;

	int		m_iLoadingCountMax;
	int		m_iLoadingCount;

	float	m_fLoadPercentMax;
	float	m_fLoadPercentTexMax;
	float	m_fLoadPercentAll;
	struct sLoadingVl
	{
		float	fLoadPercentAll;
		float	fLoadPercentVal;
	};
	typedef std::map<int, sLoadingVl> MapLoadingVal;
	MapLoadingVal	m_mapLoadingVal;

	bool m_bLoadingOk;
};

#endif