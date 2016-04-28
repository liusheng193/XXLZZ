#include "cfg_config.h"
#include "cocostudio/CocoStudio.h"

//#include "utility.h"

USING_NS_CC;
using namespace cocostudio;

CCfg_config* CCfg_config::s_instance = nullptr;

//ππ‘Ï
CCfg_config::CCfg_config()
{
}
CCfg_config::~CCfg_config()
{
	
	/*for(auto &v:m_vMonsterllist)
		delete v;*/
}
CCfg_config* CCfg_config::getInstance()
{
	if(s_instance)return s_instance;
	s_instance=new CCfg_config();
	return s_instance;
}

void CCfg_config::destroyInstande()
{
	if(s_instance)
		delete s_instance;
}

bool CCfg_config::loadConfig_mapBase()
{
	return true;
}
bool CCfg_config::loadConfig_main(const std::string& file)
{
	return true;
}