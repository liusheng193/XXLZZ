#ifndef __CFG_CONFIG_H__
#define __CFG_CONFIG_H__
#include "cocos2d.h"
#include "Config.h"

//配置表
class CCfg_config
{
public:
	CCfg_config();
	~CCfg_config();
	//获取句柄
	static CCfg_config* getInstance();
	//销毁
	static void destroyInstande();
	//load
public:
	//加载地图基础配置
	bool loadConfig_mapBase();
	//加载配置表
	bool loadConfig_main(const std::string& file);
public:

protected:
	static CCfg_config*		s_instance;
public:

};

#endif 