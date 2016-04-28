#ifndef __CFG_CONFIG_H__
#define __CFG_CONFIG_H__
#include "cocos2d.h"
#include "Config.h"

//���ñ�
class CCfg_config
{
public:
	CCfg_config();
	~CCfg_config();
	//��ȡ���
	static CCfg_config* getInstance();
	//����
	static void destroyInstande();
	//load
public:
	//���ص�ͼ��������
	bool loadConfig_mapBase();
	//�������ñ�
	bool loadConfig_main(const std::string& file);
public:

protected:
	static CCfg_config*		s_instance;
public:

};

#endif 