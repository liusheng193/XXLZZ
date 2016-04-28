#ifndef __CD_PRELOAD_H__
#define __CD_PRELOAD_H__
// by cgq 2015-4-22

#include "cocos2d.h"

typedef std::function<int(const std::string&, const std::string&, int)> datfuncCallBack;
typedef std::function<void(int)> datfuncCallBackEnd;

enum emPreloadType
{
	PreLoadType_EnyDat,			// 加载.dat数据
	PreLoadType_Callback,		// 外部加载
};

// 预加载
class CPreload
{
public:
	static CPreload* getInstance(void);
	static CPreload* createInstance(void);
	static void destroyInstance(void);

	// 预加载数据
	static void	preloadDat(const std::string& filename, const datfuncCallBack& callback = nullptr);
	static void	preloadDatAsync(const std::string& filename, const datfuncCallBack& callback = nullptr, const datfuncCallBackEnd& callbackEnd = nullptr);

	// 回调外部加载数据
	static void preloadDatAsyncCall(const std::string& filename, const datfuncCallBack& callback, const datfuncCallBackEnd& callbackEnd = nullptr);

	// 加载结束 关闭线程
	static void preloadEnd(void);
	// 卸载
	static void	removeDat(const std::string& filename);
	static void	removeAll(void);

	// 加载个数
	static int getPreloadCount(void);

protected:
	CPreload(void);

	//typedef std::vector<cocos2d::Vec2> VectVec2;
	//typedef std::map<std::string, VectVec2> MapVec2;

	typedef std::vector<std::string> VectString;
	typedef std::map<std::string, VectString> MapVectString;
	struct sDat 
	{
		VectString vPng;
		MapVectString vPlist;
		VectString vEffect;
		VectString vMusic;
		//MapVec2 vPath;
	};
	typedef std::map<std::string, sDat>	MapDat;
	MapDat	m_mapDat;

	// call back
	struct sCallBack
	{
	public:
		sCallBack(datfuncCallBack f, datfuncCallBackEnd fEnd, emPreloadType pretype) 
			: callback(f), callbackEnd(fEnd), type(pretype), retCode(0){}
		sCallBack(const sCallBack& o):callback(o.callback), callbackEnd(o.callbackEnd), type(o.type), retCode(0){}

		emPreloadType type;
		datfuncCallBack	callback;
		datfuncCallBackEnd	callbackEnd;
		int retCode;
	};

	// 多线程加载
	struct AsyncStruct
	{
	public:
		AsyncStruct(const std::string& fn, const std::string& fnPath, const sCallBack& call) 
			: ref(0), filename(fn), filenamePath(fnPath), callBack(call) {}

		void addRef(void){++ref;}
		void delRef(void){--ref;}

		int	ref;
		std::string filename;
		std::string filenamePath;
		sCallBack callBack;
	};

	typedef struct _ImageInfo
	{
		AsyncStruct *asyncStruct;
		cocos2d::Image *image;
		std::string	 filename;
		std::string	 datbuf;
		int type;
		VectString* valMap;
	} ImageInfo;

	struct sPlistInfo 
	{
		AsyncStruct *asyncStruct;
		std::string	 filename;
		int	count;
	};

	std::thread* m_loadingThread;

	std::queue<AsyncStruct*>* m_asyncStructQueue;
	std::deque<ImageInfo*>* m_imageInfoQueue;
	std::deque<sPlistInfo*>* m_plistInfoQueue;

	std::mutex m_asyncStructQueueMutex;
	std::mutex m_imageInfoMutex;
	std::mutex m_plistInfoMutex;

	std::mutex m_sleepMutex;
	std::condition_variable m_sleepCondition;

	bool m_needQuit;
	int m_asyncRefCount;

	// 多线程处理
	struct sValueMap 
	{
		std::string name;
		cocos2d::Rect frame;
		bool rotated;
		cocos2d::Vec2 offset;
		cocos2d::Size sourceSize;
	};
	typedef std::vector<sValueMap*> VectValueMap;
	struct sSpriteFrameAsync
	{
		std::string name;
		std::string nameTex;
		VectValueMap* vMap;
	};
	std::deque<sSpriteFrameAsync*>* m_spriteFramesQueue;
	std::mutex m_spriteFramesMutex;

	// 单线程处理
	struct sSpriteFrame
	{
		std::string name;
		cocos2d::SpriteFrame* sf;
	};
	typedef std::vector<sSpriteFrame> VectSpriteFrame;
	typedef std::map<std::string, VectSpriteFrame> MapSpriteFrame;
	MapSpriteFrame m_spriteFramesDx;

	// head
	struct sDatHead 
	{
		int cout;
		int type;
		char eyCharStr;
		char eyCharInt;
	};

protected:
	// load
	void		_loadDat(const std::string& filename, const sCallBack& call);
	void		_loadDatAsync(const std::string& filename, const sCallBack& call);
	// 线程回调
	void		_loadDatAsyncThread(void);

	// callback
	void		_loadDatAsyncCallBack(float dt);

	// 读取数据头
	sDatHead	_readHead(unsigned char* bytes, int &jps);

protected:
	// add
	void		_addSpriteFramesAsync(sSpriteFrameAsync* sfAsy, cocos2d::Texture2D* tex, MapVectString& vPlist);

	// 读取spritFrame
	void		_loaddSpriteFramesAsync(sDatHead &datHead, AsyncStruct* ast, std::string& path2, unsigned char* bytes, int &jps);
	void		_loaddSpriteFrames(sDatHead &datHead, std::string& path, std::string& path2, unsigned char* bytes, int &jps, MapVectString& vPlist, const sCallBack& call);
};

#endif /* __CD_PRELOAD_H__ */
