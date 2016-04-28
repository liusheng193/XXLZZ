#ifndef __SCENE_MANAGE_H__
#define __SCENE_MANAGE_H__
//��������
#include "Config.h"

class CScene_loading;
class CSceneManage :public cocos2d::Node
{
public:
	~CSceneManage(void);
	CREATE_FUNC(CSceneManage);
	static CSceneManage* getInstance(void);

	// ��ʼ��
	virtual bool init(void);

public:
	// ��������
	cocos2d::Scene* createScene(emSceneType type);
	void	ingame(void);
protected:
	// ��Ϸ�����˳�
	void _editGame(void);
	// ��Ϸ���
	void _checkGame(void);

	// ��ʼ��Ϸ
	void _startGame(void);

	// ������Դ
	void _loadResource(void);
protected:
	static CSceneManage* s_sceneManage;
	emSceneType			m_emSceneType;
	cocos2d::Layer*		m_curLayer;
	cocos2d::Scene*		m_curScene;
	OkCallFunc			m_callfunc_start;

	CScene_loading*		m_loadScene;			// ���ؽ���
	bool				m_bExit;				// �˳�

};


#endif