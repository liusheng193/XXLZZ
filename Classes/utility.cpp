#include "utility.h"
#include "cocostudio/CocoStudio.h"
#include "cd_visibleRect.h"
#include "layer_mask.h"
USING_NS_CC;

static thUtility* g_instance = nullptr;

thUtility::thUtility()
	: m_curMessageBoxId(-1)
{
}
thUtility::~thUtility()
{
	m_mapString.clear();
	g_instance = nullptr;
}

thUtility* thUtility::getInstance()
{
	if(g_instance==nullptr)
		g_instance=new thUtility();
	return g_instance;
}

void thUtility::destroyInstance()
{
	if(g_instance)
	{
		delete g_instance;
		g_instance=nullptr;
	}
}

void thUtility::pushString(int id, const std::string& str)
{
	if(!g_instance) return;
	g_instance->m_mapString.insert(MapString::value_type(id,str));
}

void thUtility::popString(int id)
{
	if(!g_instance) return;
	auto iter=g_instance->m_mapString.find(id);
	if(iter!=g_instance->m_mapString.end())
	{
		g_instance->m_mapString.erase(iter);
	}
}

std::string thUtility::getString(int id)
{
	if(!g_instance)return "";
	auto iter=g_instance->m_mapString.find(id);
	if(iter!=g_instance->m_mapString.end())
	{
		return iter->second;
	}
	return "";
}

void thUtility::loadStringFile(const std::string& file)
{
	auto vMapString=FileUtils::getInstance()->getValueMapFromFile(file);
	for(auto &v:vMapString)
	{
		__String val=v.first;
		pushString(val.intValue(),v.second.asString());
	}
}

void thUtility::analyTime(int time, sTime &out)
{
	out.day = time/60/60/24;
	int tmp1 = time - out.day*60*60*24;
	out.hour = tmp1/60/60;
	int tmp2 = time - out.hour*60*60 - out.day*60*60*24;
	out.min = tmp2/60;
	out.second = time - out.min*60 - out.hour*60*60 - out.day*60*60*24;
}

void thUtility::formatTime(int timedl,char *szbuf,int size)
{
	sTime time;
	analyTime(timedl,time);
	
	char _szBuf[126];
	std::string str;
	if(time.day!=0)
	{
		snprintf(_szBuf,126,"%d%s",time.day,getString(STRING_ID_TIME_DAY).c_str());
		str.append(_szBuf);
	}
	if (time.hour != 0)
	{
		snprintf(_szBuf, 126, "%d%s", time.hour, thUtility::getString(STRING_ID_TIME_HOUR).c_str());
		str.append(_szBuf);
	}
	if (time.min != 0)
	{
		snprintf(_szBuf, 126, "%d%s", time.min, thUtility::getString(STRING_ID_TIME_MIN).c_str());
		str.append(_szBuf);
	}
	if (time.second != 0)
	{
		snprintf(_szBuf, 126, "%d%s", time.second, thUtility::getString(STRING_ID_TIME_SECOND).c_str());
		str.append(_szBuf);
 	}
	if (str.length() > size) CC_ASSERT(false);
	memset(szbuf, 0, size);
	strncpy(szbuf, str.data(), str.length());
}

void thUtility::addMessageBox( int id,cocos2d::Node* nd,VectorInt& vBtnid)
{
	if(!g_instance)return;

	auto &_mapMessageBox=g_instance->m_mapMessageBox;
	auto iter=_mapMessageBox.find(id);
	if(iter!=_mapMessageBox.end())
		return;
	if(nd)
	{
		nd->setVisible(false);
		for(auto &v: vBtnid)
		{
			auto btn=dynamic_cast<ui::Button*>(nd->getChildByTag(v));
			btn->addTouchEventListener(CC_CALLBACK_2(thUtility::messageBoxCallBack,g_instance));
		}
		sMessageBox box;
		box.messagebox=nd;
		_mapMessageBox.insert(MapMessageBox::value_type(id,box));
	}
}

void thUtility::removeMessageBox(int id)
{
	if(!g_instance)return ;
	auto &_mapMessageBox=g_instance->m_mapMessageBox;
	auto iter=_mapMessageBox.find(id);
	if(iter!=_mapMessageBox.end())
	{
		_mapMessageBox.erase(iter);
	}
}

void thUtility::setMessageBoxTitle(const std::string& str,int id)
{
	if(!g_instance)return ;
	if(id==-1) id=g_instance->m_curMessageBoxId;
	auto &_mapMessageBox=g_instance->m_mapMessageBox;
	auto iter=_mapMessageBox.find(id);
	if(iter!=_mapMessageBox.end())
	{
		auto messagebox=iter->second.messagebox;
		auto txt=dynamic_cast<ui::Text*>(messagebox->getChildByTag(100));
		txt->setString(str);
	}
}

void thUtility::setMessageBoxString(const std::string& str,int id)
{
	
	if(!g_instance)return ;
	if(id==-1) id=g_instance->m_curMessageBoxId;
	auto &_mapMessageBox=g_instance->m_mapMessageBox;
	auto iter=_mapMessageBox.find(id);
	if(iter!=_mapMessageBox.end())
	{
		auto messagebox=iter->second.messagebox;
		auto txt=dynamic_cast<ui::Text*>(messagebox->getChildByTag(101));
		txt->setString(str);
	}
}

void thUtility::addMessageBoxCallBack(int id,const funcMessageBoxCallBack& callback,int btnid,bool bPop)
{
	if(!g_instance)return ;
	auto &_mapMessageBox = g_instance->m_mapMessageBox;
	auto iter = _mapMessageBox.find(id);
	if (iter != _mapMessageBox.end())
	{
		sMessageBoxCall box;
		box.callfunc=callback;
		box.btnID=btnid;
		box.bPop=bPop;
		iter->second.vFuncCallBack.push_back(box);
	}
}

void thUtility::showMessageBox( int id, bool bShow )
{
	if (!g_instance) return;

	auto &_mapMessageBox = g_instance->m_mapMessageBox;
	auto iter = _mapMessageBox.find(id);
	if (iter != _mapMessageBox.end())
	{
		auto messagebox=iter->second.messagebox;
		messagebox->setVisible(bShow);
		auto nd = messagebox->getChildByTag(99);
		if (bShow) 
		{
			g_instance->m_curMessageBoxId = id;
			auto ly = CLayer_mask::pushMask(nd, 0);
			ly->setPosition(-ly->getContentSize().width/2, -ly->getContentSize().height/2);
		}
		else 
		{
			CLayer_mask::popMask(nd,0);
			g_instance->m_curMessageBoxId = -1;
		}
	}
}
void thUtility::showMessageBox( int id, bool bShow, const std::string& str )
{
	showMessageBox(id, bShow);
	setMessageBoxString(str, id);
}

void thUtility::messageBoxCallBack( cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type )
{
	auto node=dynamic_cast<Node*>(ref);
	if(type==ui::Widget::TouchEventType::BEGAN)
	{
		thUtility::beginButtonScaleEffect(node);
	}
	else if(type==ui::Widget::TouchEventType::ENDED)
	{
		thUtility::endButtonScaleEffect(node);
		int id=node->getTag();
		auto iter = m_mapMessageBox.find(m_curMessageBoxId);
		if (iter != m_mapMessageBox.end())
		{
			auto &vFuncCallBack=iter->second.vFuncCallBack;
			auto iterCall=vFuncCallBack.begin();
			while (iterCall != vFuncCallBack.end())
			{
				auto &call = *iterCall;
				if(call.btnID==id)
				{
					call.callfunc(m_curMessageBoxId,id);
					if(call.bPop)
					{
						iterCall=vFuncCallBack.erase(iterCall);
						continue;
					}
				}
				++iterCall;
			}
		}
	}
	else if (type == ui::Widget::TouchEventType::CANCELED)
	{
		thUtility::endButtonScaleEffect(node);
	}
}


void thUtility::analyStrToStr( std::string &str, char tp, const std::string &to )
{
	int id = str.find(tp);
	str.erase(id, 1);
	str.insert(id, to.c_str());
}

std::string thUtility::analyString( std::string &str, char tp )
{
	int id = str.find(tp);
	if (id != -1)
	{
		std::string ret = str.substr(0, id);
		str = str.substr(id+1);
		return ret;
	}
	return "";
}

int thUtility::analyInt( std::string &str, char type )
{
	int id = str.find(type);
	if (id != -1)
	{
		__String val = str.substr(0, id);
		str = str.substr(id+1);
		return val.intValue();
	}
	if (str.empty())
	{
		return -1;
	}
	__String val = str;
	str = "";
	return val.intValue();
}

float thUtility::analyFloat( std::string &str, char type )
{
	int id = str.find(type);
	if (id != -1)
	{
		__String val = str.substr(0, id);
		str = str.substr(id+1);
		return val.floatValue();
	}
	if (str.empty())
	{
		return -1;
	}
	__String val = str;
	return val.floatValue();
}
void thUtility::setButtonEnable( cocos2d::Node* btn, bool enabled )
{
	if (!btn) return;
	auto btnptr = dynamic_cast<ui::Button*>(btn);
	btnptr->setTouchEnabled(enabled);
	btnptr->setEnabled(enabled);
	btnptr->setBright(enabled);
}

std::string thUtility::getFileName( const std::string& str )
{
	int id = str.find_last_of(".");
	if (id != -1)
	{
		return str.substr(0, id);
	}
	return "";
}


void thUtility::beginButtonScaleEffect( cocos2d::Node* btn )
{
	btn->setScale(btn->getScale()-0.1f);
}

void thUtility::endButtonScaleEffect( cocos2d::Node* btn )
{
	btn->setScale(btn->getScale()+0.1f);
}
