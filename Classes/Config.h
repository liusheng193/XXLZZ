//
// Config--设置
//
//		Copyright (c) 2000-2001 Chihiro.SAKAMOTO (HyperWorks)
//
#ifndef	__config_h__
#define	__config_h__
#include "cocos2d.h"


#define	CompanyName			"HyperWorks"
#define	ApplicationName		"RPGSYSTEM"
#define	ApplicationTitle	"RPGSample"

#define	WindowWidth		960
#define	WindowHeight	640

#define	CGPATH			"cgdata/"
#define	SCRIPTPATH		"script/"
#define	WAVEPATH		"wave/"
#define	MAPPATH			"map/"
#define	PARAMETERFILE	"map/parameter.dat"
#define	SCENCEPATH		"Scene/"
#define	BATTLEPATH		"Battle/"
#define	CONFIGPATH		"Data/Config/"
#define	UIPATH			"UI/"
#define	XMLPATH			"Data/"
#define FONTPATH		"fonts/simhei.ttf"


#define	MessageFont		16
#define	MessageStyle	FW_BOLD
#define	MessageWidth	70
#define	MessageLine		4

#define	PARAMS_MAX_SAVE		10
#define	PARAMS_MAX_VALUES	100

// 定义最大的角色数
#define	MAX_CHARACTER	3

#define	MAX_MAGIC		2

//


// log
#define LOG_ID_GAME				0
// str id
#define STRING_ID_TIME_DAY		90
#define STRING_ID_TIME_HOUR		91
#define STRING_ID_TIME_MIN		92
#define STRING_ID_TIME_SECOND	93

//遮罩
#define TAG_MAIN_UI_PROPU	202
#define TAG_MAIN_UI_PROP	201
#define TAG_MAIN_UI_MASK	200
#define TAG_MAIN_UI			199

//消息
enum PosNType
{
	PS_FIRSTACTION,		//最初传送的事件
	PS_CLOSE,			//关闭窗口
	PS_ERASEBKGND,		//删除背景
	PS_LBUTTONDOWN,		//按下鼠标左键
	PS_LBUTTONUP,		//放开鼠标左键
	PS_RBUTTONDOWN,		//按下鼠标右键
	PS_RBUTTONUP,		//放开鼠标右键
	PS_MOUSEMOVE,		//鼠标指标移动
	PS_KEYDOWN,			//按下键盘
	PS_DESTROY,			//取消窗口
	PS_TIMER,			//计时器到期
	PS_MCINOTIFY,		//MCI的事件
	PS_IDLE				//空闲事件
};
	



#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define C_STRICMP  _stricmp 
#else
#define C_STRICMP strcasecmp 
#endif

enum emSceneType
{
	Scene_note,
	Scene_logo,				// logo
	Scene_loading,			// 加载界面
	Scene_regame,			// 重新加载游戏
	Scene_menu,				// 菜单界面
	Scene_mapmini,			// 小地图
	Scene_main,				// 主界面场景
};


enum emLoadingType
{
	LoadingType_ani,
	LoadingType_ani_wujian,
	LoadingType_ui,
	LoadingType_ui_map,
	LoadingType_ui_map_wj,
	LoadingType_ui_shop,
	LoadingType_ui_battle,
	LoadingType_battlefieldAI,
	LoadingType_battlefieldAI_Soldiers,
	LoadingType_battlefieldAI_UI_AimTarget,

	LoadingType_cfg_base,
	LoadingType_cfg_main,
	LoadingType_cfg_defence,
	LoadingType_cfg_builder,
	LoadingType_cfg_tribalcastle,
	LoadingType_cfg_army,
	LoadingType_cfg_resourse,
	LoadingType_cfg_wall,
	LoadingType_cfg_camp,
	LoadingType_cfg_train,
	LoadingType_cfg_magicfactory,
	LoadingType_cfg_soldier,
	LoadingType_cfg_magic,
	LoadingType_cfg_lab,
	LoadingType_cfg_exchange,
	LoadingType_cfg_pircerise,
	LoadingType_cfg_shield,
	LoadingType_cfg_trap,
	LoadingType_cfg_ornament,
	LoadingType_cfg_obstacle,
	LoadingType_cfg_playerlevel,
	LoadingType_cfg_achievement,
	LoadingType_cfg_checkpoint,

	// network
	LoadingType_network_connect,
};

// function
#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)
typedef std::function<void(void)> OkCallFunc;
typedef std::function<bool(void)> OkRetCallFunc;
typedef std::function<void(int)> IntCallFunc;
typedef std::function<void(cocos2d::Node* obj)> OsyObjCallFunc;
typedef std::function<void(int type, cocos2d::Node* obj)> CallFunc_OpeDlg;
typedef std::function<void(bool bFull, int id)> CallFunc_TrainingBase;
typedef std::function<void(int id, float ftime, float ftimeMax, float ftimeAll)> CallFunc_TrainingBaseTime;
// type
typedef std::vector<cocos2d::Node*> VectorNode;
typedef std::vector<int> VectorInt;
typedef std::map<int, int> MapIntInt;

typedef unsigned int        UINT;
// time
struct sTime
{
	int			day;				// 天
	int			hour;				// 小时
	int			min;				// 分钟
	int			second;				// 秒
};
#include "function.h"
//#include "less.h"
#endif
