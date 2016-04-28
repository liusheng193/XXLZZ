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

	// ��ʼ��
	virtual bool init(void);
	// ����
	virtual void onEnter(void);
	// �ͷ���Դ
	virtual void onExit(void);

public:
	// ������Ϣ�ص�
	void setStartGameMsg(const std::function<void(void)>& callback);
	// ���ý���
	void setPercent(float pt);
	void addPercent(float pt);
	float getPercent(void){ return m_fLoadPercentAll; }
	// �Ƿ����ok
	bool isLoadingOk(void){ return m_bLoadingOk; }
	// ��������
	void popData(void);

	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
protected:
	// ������Դ
	void _loading(void);
	// ��Դ���ػص�
	int _loadingResCallBack(const std::string& name, const std::string& buf, int count, int type);
	void _loadingResEndCallBack(int);
	void _loadingResEndCallBackMessage(int);

	// �����ļ�����
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