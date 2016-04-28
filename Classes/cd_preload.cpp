#include "cd_preload.h"

#include "SimpleAudioEngine.h"
#include "cocostudio/CCSpriteFrameCacheHelper.h"

#include <errno.h>
#include <stack>
#include <cctype>
#include <list>

#include <queue>
#include <deque>

USING_NS_CC;
using namespace CocosDenshion;

std::string decryptionString(char* str, int len, char eyChar)
{
	std::string strDec;
	for (int i=0; i<len; ++i)
	{
		char s = str[i];
		s = s - eyChar;
		s = ~s;
		strDec.push_back(s);
	}
	return strDec;
}

int decryptionInt(int val, char eyChar)
{
	val = val - eyChar;
	val = ~val;
	return val;
}

static CPreload* _sharedCPreload = nullptr;
CPreload::CPreload( void )
	: m_loadingThread(nullptr)
	, m_asyncStructQueue(nullptr)
	, m_imageInfoQueue(nullptr)
	, m_plistInfoQueue(nullptr)
	, m_asyncRefCount(0)
	, m_needQuit(false)
{

}

CPreload* CPreload::getInstance( void )
{
	return _sharedCPreload;
}

CPreload* CPreload::createInstance( void )
{
	if (!_sharedCPreload)
	{
		_sharedCPreload = new CPreload();
	}
	return _sharedCPreload;
}

void CPreload::destroyInstance( void )
{
	removeAll();
	CC_SAFE_DELETE(_sharedCPreload);
}

void CPreload::preloadDat( const std::string& filename, const datfuncCallBack& callback )
{
	sCallBack call(callback, nullptr, PreLoadType_EnyDat);
	_sharedCPreload->_loadDat(filename, call);
}

void CPreload::preloadDatAsync( const std::string& filename, const datfuncCallBack& callback, const datfuncCallBackEnd& callbackEnd )
{
	sCallBack call(callback, callbackEnd, PreLoadType_EnyDat);
	_sharedCPreload->_loadDatAsync(filename, call);
}

void CPreload::preloadDatAsyncCall( const std::string& filename, const datfuncCallBack& callback, const datfuncCallBackEnd& callbackEnd )
{
	if (!_sharedCPreload) return;
	sCallBack call(callback, callbackEnd, PreLoadType_Callback);
	_sharedCPreload->_loadDatAsync(filename, call);
}

void CPreload::preloadEnd( void )
{
	_sharedCPreload->m_asyncRefCount = 0;
	Director::getInstance()->getScheduler()->unschedule("refCount", _sharedCPreload);

	_sharedCPreload->m_needQuit = true;
	_sharedCPreload->m_sleepCondition.notify_one();
	if (_sharedCPreload->m_loadingThread) _sharedCPreload->m_loadingThread->join();
	CC_SAFE_DELETE(_sharedCPreload->m_loadingThread);
	_sharedCPreload->m_loadingThread = nullptr;
}

void CPreload::removeDat( const std::string& filename )
{
	if (!_sharedCPreload) return;

	const std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);

	MapDat::iterator iter = _sharedCPreload->m_mapDat.find(fullPath);
	if (iter != _sharedCPreload->m_mapDat.end())
	{
		auto __spriteFrameCache = SpriteFrameCache::getInstance();
		auto __textureCache = Director::getInstance()->getTextureCache();
		auto __simpleAudioEngine = SimpleAudioEngine::getInstance();
		auto &v = iter->second;
		for (auto &plist : v.vPlist)
		{
			__spriteFrameCache->removeSpriteFrameByPlistVct(plist.first, plist.second);
		}
		for (auto &png : v.vPng)
		{
			__textureCache->removeTextureForKey(png);
		}
		for (auto &effect : v.vEffect)
		{
			__simpleAudioEngine->unloadEffect(effect.c_str());
		}
// 		for (auto &music : v.vMusic)
// 		{
// 			__simpleAudioEngine->unloadEffect(effect.c_str());
// 		}

		_sharedCPreload->m_mapDat.erase(iter);
	}
}

void CPreload::removeAll( void )
{
	preloadEnd();

	auto __spriteFrameCache = SpriteFrameCache::getInstance();
	auto __textureCache = Director::getInstance()->getTextureCache();
	auto __simpleAudioEngine = SimpleAudioEngine::getInstance();

	MapDat::iterator iter = _sharedCPreload->m_mapDat.begin();
	while (iter != _sharedCPreload->m_mapDat.end())
	{
		auto &v = iter->second;
		for (auto &plist : v.vPlist)
		{
			__spriteFrameCache->removeSpriteFrameByPlistVct(plist.first, plist.second);
		}
		for (auto &png : v.vPng)
		{
			__textureCache->removeTextureForKey(png);
		}
		for (auto &effect : v.vEffect)
		{
			__simpleAudioEngine->unloadEffect(effect.c_str());
		}
		++iter;
	}
	_sharedCPreload->m_mapDat.clear();
	_sharedCPreload->m_spriteFramesDx.clear();
}

void CPreload::_loadDat( const std::string& filename, const sCallBack& call )
{
	int idot = filename.rfind("/");
	if (idot == -1) filename.rfind("\\");
	std::string path1 = filename.substr(0, idot+1);

	const std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
	if (fullPath.empty())
	{
		return;
	}

	auto it = m_mapDat.find(fullPath);
	if(it != m_mapDat.end())
	{
		return;
	}

	idot = fullPath.rfind("/");
	if (idot == -1) fullPath.rfind("\\");
	std::string path2 = fullPath.substr(0, idot+1);

	auto dat = FileUtils::getInstance()->getDataFromFile(fullPath);
	if (!dat.isNull())
	{
		sDat vDat;
		auto bytes = dat.getBytes();

		int jps = 0;
		auto datHead = _readHead(bytes, jps);

		// call back
		if (call.callback) call.callback(filename, "", datHead.cout);

		// 图像
		if (datHead.type == 0)
		{
			// pilst
			_sharedCPreload->m_spriteFramesDx.clear();
			_sharedCPreload->_loaddSpriteFrames(datHead, path2, path1, bytes, jps, vDat.vPlist, call);

			// 加载png
			auto __textureCache = Director::getInstance()->getTextureCache();
			int pngsize = *(int*)(bytes+jps); jps += sizeof(int);
			pngsize = decryptionInt(pngsize, datHead.eyCharInt);
			for (int i=0; i<pngsize; ++i)
			{
				int len = *(int*)(bytes+jps); jps += sizeof(int);
				len = decryptionInt(len, datHead.eyCharInt);
				char *strptr = (char*)(bytes+jps); jps += len;

				// 解密
				std::string str = path1 + decryptionString(strptr, len, datHead.eyCharStr);

				// image data
				len = *(int*)(bytes+jps); jps += sizeof(int);
				len = decryptionInt(len, datHead.eyCharInt);
				unsigned char* datptr = (unsigned char*)(bytes+jps); jps += len;

				// generate image      
				auto image = new (std::nothrow) Image();
				if (image && !image->initWithImageData(datptr, len))
				{
					CC_SAFE_RELEASE_NULL(image);
					CCLOG("can not load %s", str.c_str());
					// continue;
				}
				//	image->saveToFile(str,false);
				Texture2D *texture = __textureCache->addImage(image, str);
				image->release();

				vDat.vPng.push_back(str);
				if (call.callback) call.callback(str, "", 0);
			}

			// 设置图片
			auto __spriteFrameCache = SpriteFrameCache::getInstance();
			MapSpriteFrame::iterator iter = _sharedCPreload->m_spriteFramesDx.begin();
			while (iter != _sharedCPreload->m_spriteFramesDx.end())
			{
				auto texture = __textureCache->getTextureForKey(iter->first);
				VectSpriteFrame &vSpriteFrame = iter->second;
				for (auto &v : vSpriteFrame)
				{
					v.sf->setTexture(texture);
				}
				++iter;
			}
			_sharedCPreload->m_spriteFramesDx.clear();
		}
		// 配置文件json ...
		else if (datHead.type == 4)
		{
			auto __fileUtils = FileUtils::getInstance();
			// 加载配置文件
			int size = *(int*)(bytes+jps); jps += sizeof(int);
			size = decryptionInt(size, datHead.eyCharInt);
			for (int i=0; i<size; ++i)
			{
				int len = *(int*)(bytes+jps); jps += sizeof(int);
				len = decryptionInt(len, datHead.eyCharInt);
				char *strptr = (char*)(bytes+jps); jps += len;

				// 解密
				auto strname = decryptionString(strptr, len, datHead.eyCharStr);

				// data
				len = *(int*)(bytes+jps); jps += sizeof(int);
				len = decryptionInt(len, datHead.eyCharInt);
				char* buf = (char*)(bytes+jps); jps += len;
				// 解密
				std::string datptr = decryptionString(buf, len, datHead.eyCharStr);

				// call
				if (call.callback) call.callback(strname, datptr, 0);
			}
		}
		// 声效
		else if (datHead.type == 1)
		{
			auto __fileUtils = FileUtils::getInstance();
			auto __simpleAudioEngine = SimpleAudioEngine::getInstance();
			// 加载音效
			int size = *(int*)(bytes+jps); jps += sizeof(int);
			size = decryptionInt(size, datHead.eyCharInt);
			for (int i=0; i<size; ++i)
			{
				int len = *(int*)(bytes+jps); jps += sizeof(int);
				len = decryptionInt(len, datHead.eyCharInt);
				char *strptr = (char*)(bytes+jps); jps += len;

				// 解密
				std::string str = path2 + decryptionString(strptr, len, datHead.eyCharStr);

				auto vValueMap = __fileUtils->getValueMapFromFile(str);
				for (auto &v : vValueMap)
				{
					__simpleAudioEngine->preloadEffect(v.second.asString().c_str());
					vDat.vEffect.push_back(v.second.asString());
				}

				// call
				if (call.callback) call.callback(str, "", 0);
			}
		}
		// 音乐
		else if (datHead.type == 2)
		{
			auto __fileUtils = FileUtils::getInstance();
			auto __simpleAudioEngine = SimpleAudioEngine::getInstance();
			// 加载音效
			int size = *(int*)(bytes+jps); jps += sizeof(int);
			size = decryptionInt(size, datHead.eyCharInt);
			for (int i=0; i<size; ++i)
			{
				int len = *(int*)(bytes+jps); jps += sizeof(int);
				len = decryptionInt(len, datHead.eyCharInt);
				char *strptr = (char*)(bytes+jps); jps += len;

				// 解密
				std::string str = path2 + decryptionString(strptr, len, datHead.eyCharStr);

				auto vValueMap = __fileUtils->getValueMapFromFile(str);
				for (auto &v : vValueMap)
				{
					__simpleAudioEngine->preloadBackgroundMusic(v.second.asString().c_str());
					vDat.vEffect.push_back(v.second.asString());
				}

				// call
				if (call.callback) call.callback(str, "", 0);
			}
		}

		// add
		_sharedCPreload->m_mapDat.insert(MapDat::value_type(fullPath, vDat));
	}
}

void CPreload::_loadDatAsync( const std::string& filename, const sCallBack& call )
{
	int idot = filename.rfind("/");
	if (idot == -1) filename.rfind("\\");
	std::string path1 = filename.substr(0, idot+1);

	const std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);

	auto it = m_mapDat.find(fullPath);
	if(it != m_mapDat.end())
	{
		return;
	}

	// lazy init
	if (m_asyncStructQueue == nullptr)
	{             
		m_asyncStructQueue = new std::queue<AsyncStruct*>();
		m_imageInfoQueue   = new std::deque<ImageInfo*>();  
		m_plistInfoQueue   = new std::deque<sPlistInfo*>();
		m_spriteFramesQueue = new std::deque<sSpriteFrameAsync*>();

		// create a new thread to load images
		m_loadingThread = new std::thread(&CPreload::_loadDatAsyncThread, this);

		m_needQuit = false;
	}

	if (0 == m_asyncRefCount)
	{
		Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(CPreload::_loadDatAsyncCallBack, this), this, 0, false, "refCount");
	}

	++m_asyncRefCount;

	// generate async struct
	AsyncStruct *data = new (std::nothrow) AsyncStruct(fullPath, path1, call);

	// add async struct into queue
	m_asyncStructQueueMutex.lock();
	m_asyncStructQueue->push(data);
	m_asyncStructQueueMutex.unlock();

	m_sleepCondition.notify_one();
}

void CPreload::_loadDatAsyncThread( void )
{
	AsyncStruct *asyncStruct = nullptr;

	while (true)
	{
		std::queue<AsyncStruct*> *pQueue = m_asyncStructQueue;
		m_asyncStructQueueMutex.lock();
		if (pQueue->empty())
		{
			m_asyncStructQueueMutex.unlock();
			if (m_needQuit) 
			{
				break;
			}
			else 
			{
				std::unique_lock<std::mutex> lk(m_sleepMutex);
				m_sleepCondition.wait(lk);
				continue;
			}
		}
		else
		{
			asyncStruct = pQueue->front();
			pQueue->pop();
			m_asyncStructQueueMutex.unlock();
		}        

		{
			const std::string& filename = asyncStruct->filename;
			if (asyncStruct->callBack.type == PreLoadType_Callback)
			{
				if (asyncStruct->callBack.callback)
					asyncStruct->callBack.retCode = asyncStruct->callBack.callback(filename, "", 0);

				sPlistInfo *plist = new sPlistInfo;
				plist->asyncStruct = asyncStruct;
				plist->filename = filename;
				plist->count = 0;

				m_plistInfoMutex.lock();
				m_plistInfoQueue->push_back(plist);
				m_plistInfoMutex.unlock();
				continue;
			}

			// PreLoadType_EnyDat
			int idot = filename.rfind("/");
			if (idot == -1) filename.rfind("\\");
			std::string path = asyncStruct->filename.substr(0, idot+1);

			auto dat = FileUtils::getInstance()->getDataFromFile(filename);
			if (dat.isNull())
			{
				// 文件是空的
				sPlistInfo *plist = new sPlistInfo;
				plist->asyncStruct = asyncStruct;
				plist->filename = filename;
				plist->count = -1;

				m_plistInfoMutex.lock();
				m_plistInfoQueue->push_back(plist);
				m_plistInfoMutex.unlock();
				continue;
			}
	//		if (!dat.isNull())
			{
				//sDat vDat;
				auto bytes = dat.getBytes();

				int jps = 0;
				auto datHead = _readHead(bytes, jps);

				// call back
				{
					sPlistInfo *plist = new sPlistInfo;
					plist->asyncStruct = asyncStruct;/* ++asyncStruct->ref;*/
					plist->filename = filename;
					plist->count = datHead.cout;

					m_plistInfoMutex.lock();
					m_plistInfoQueue->push_back(plist);
					m_plistInfoMutex.unlock();
				}

				if (datHead.type == 0)
				{
					// pilst
					_loaddSpriteFramesAsync(datHead, asyncStruct, asyncStruct->filenamePath, bytes, jps);

					// 加载png
					int pngsize = *(int*)(bytes+jps); jps += sizeof(int);
					pngsize = decryptionInt(pngsize, datHead.eyCharInt);

					m_plistInfoMutex.lock();
					asyncStruct->ref += pngsize;
					m_plistInfoMutex.unlock();

					for (int i=0; i<pngsize; ++i)
					{
						int len = *(int*)(bytes+jps); jps += sizeof(int);
						len = decryptionInt(len, datHead.eyCharInt);
						char *strptr = (char*)(bytes+jps); jps += len;

						// generate image info
						ImageInfo *imageInfo = new (std::nothrow) ImageInfo();
						imageInfo->asyncStruct = asyncStruct;
						imageInfo->type = datHead.type;
						// 解密
						imageInfo->filename = asyncStruct->filenamePath+decryptionString(strptr, len, datHead.eyCharStr);

						// image data
						len = *(int*)(bytes+jps); jps += sizeof(int);
						len = decryptionInt(len, datHead.eyCharInt);
						unsigned char* datptr = (unsigned char*)(bytes+jps); jps += len;

						// generate image      
						Image *image = new (std::nothrow) Image();
						if (image && !image->initWithImageData(datptr, len))
						{
							CC_SAFE_RELEASE_NULL(image);
							CCLOG("can not load %s", imageInfo->filename.c_str());
							// continue;
						}
						imageInfo->image = image;
						imageInfo->valMap = nullptr;

						// put the image info into the queue
						m_imageInfoMutex.lock();
						m_imageInfoQueue->push_back(imageInfo);
						m_imageInfoMutex.unlock();
					}
				}
				// 配置文件json ...
				else if (datHead.type == 4)
				{
					auto __fileUtils = FileUtils::getInstance();
					// 加载配置文件
					int size = *(int*)(bytes+jps); jps += sizeof(int);
					size = decryptionInt(size, datHead.eyCharInt);

					m_plistInfoMutex.lock();
					asyncStruct->ref += size;
					m_plistInfoMutex.unlock();

					for (int i=0; i<size; ++i)
					{
						int len = *(int*)(bytes+jps); jps += sizeof(int);
						len = decryptionInt(len, datHead.eyCharInt);
						char *strptr = (char*)(bytes+jps); jps += len;

						// 解密
						auto strname = decryptionString(strptr, len, datHead.eyCharStr);

						// data
						len = *(int*)(bytes+jps); jps += sizeof(int);
						len = decryptionInt(len, datHead.eyCharInt);
						char* buf = (char*)(bytes+jps); jps += len;
						// 解密
						std::string datptr = decryptionString(buf, len, datHead.eyCharStr);
						
						ImageInfo *imageInfo = new (std::nothrow) ImageInfo();
						imageInfo->asyncStruct = asyncStruct;
						imageInfo->type = datHead.type;
						imageInfo->filename = strname;
						imageInfo->image = nullptr;
						imageInfo->valMap = nullptr;
						imageInfo->datbuf = datptr;

						m_imageInfoMutex.lock();
						m_imageInfoQueue->push_back(imageInfo);
						m_imageInfoMutex.unlock();
					}
				}
				else if (datHead.type == 1)
				{
					auto __fileUtils = FileUtils::getInstance();
					auto __simpleAudioEngine = SimpleAudioEngine::getInstance();
					// 加载音效
					int size = *(int*)(bytes+jps); jps += sizeof(int);
					size = decryptionInt(size, datHead.eyCharInt);

					m_plistInfoMutex.lock();
					asyncStruct->ref += size;
					m_plistInfoMutex.unlock();

					for (int i=0; i<size; ++i)
					{
						int len = *(int*)(bytes+jps); jps += sizeof(int);
						len = decryptionInt(len, datHead.eyCharInt);
						char *strptr = (char*)(bytes+jps); jps += len;

						// 解密
						std::string str = path+decryptionString(strptr, len, datHead.eyCharStr);
						auto vValueMap = __fileUtils->getValueMapFromFile(str);

						ImageInfo *imageInfo = new (std::nothrow) ImageInfo();
						imageInfo->asyncStruct = asyncStruct;
						imageInfo->type = datHead.type;
						imageInfo->filename = str;
						imageInfo->image = nullptr;
						imageInfo->valMap = new (std::nothrow) VectString();

						for (auto &v : vValueMap)
						{
							__simpleAudioEngine->preloadEffect(v.second.asString().c_str());
							imageInfo->valMap->push_back(v.second.asString());
						}
						m_imageInfoMutex.lock();
						m_imageInfoQueue->push_back(imageInfo);
						m_imageInfoMutex.unlock();
					}
				}
				else if (datHead.type == 2)
				{
					auto __fileUtils = FileUtils::getInstance();
					auto __simpleAudioEngine = SimpleAudioEngine::getInstance();
					// 加载音效
					int size = *(int*)(bytes+jps); jps += sizeof(int);
					size = decryptionInt(size, datHead.eyCharInt);

					m_plistInfoMutex.lock();
					asyncStruct->ref += size;
					m_plistInfoMutex.unlock();

					for (int i=0; i<size; ++i)
					{
						int len = *(int*)(bytes+jps); jps += sizeof(int);
						len = decryptionInt(len, datHead.eyCharInt);
						char *strptr = (char*)(bytes+jps); jps += len;

						// 解密
						std::string str = path+decryptionString(strptr, len, datHead.eyCharStr);
						auto vValueMap = __fileUtils->getValueMapFromFile(str);

						ImageInfo *imageInfo = new (std::nothrow) ImageInfo();
						imageInfo->asyncStruct = asyncStruct;
						imageInfo->type = datHead.type;
						imageInfo->filename = str;
						imageInfo->image = nullptr;
						imageInfo->valMap = new (std::nothrow) VectString();

						for (auto &v : vValueMap)
						{
							__simpleAudioEngine->preloadBackgroundMusic(v.second.asString().c_str());
							imageInfo->valMap->push_back(v.second.asString());
						}
						m_imageInfoMutex.lock();
						m_imageInfoQueue->push_back(imageInfo);
						m_imageInfoMutex.unlock();
					}
				}
				
			}
		}    
	}

	if(m_asyncStructQueue != nullptr)
	{
		delete m_asyncStructQueue;
		m_asyncStructQueue = nullptr;
		delete m_imageInfoQueue;
		m_imageInfoQueue = nullptr;
		delete m_plistInfoQueue;
		m_plistInfoQueue = nullptr;

		size_t infoSize = m_spriteFramesQueue->size();
		for (size_t pos = 0; pos < infoSize; ++pos)
		{
			auto framePlist = (*m_spriteFramesQueue)[pos];
			for (auto &v : *framePlist->vMap)
			{
				delete v;
			}
		}
		delete m_spriteFramesQueue;
		m_spriteFramesQueue = nullptr;
	}
}

void CPreload::_loadDatAsyncCallBack( float dt )
{
	// the plist is generated in loading thread
	std::deque<sPlistInfo*> *plistQueue = m_plistInfoQueue;
	m_plistInfoMutex.lock();
	if (plistQueue->empty())
	{
		m_plistInfoMutex.unlock();
	}
	else
	{
		sPlistInfo *plistInfo = plistQueue->front();
		plistQueue->pop_front();
		m_plistInfoMutex.unlock();

		AsyncStruct *asyncStruct = plistInfo->asyncStruct;
		if (asyncStruct->callBack.type == PreLoadType_EnyDat)
		{
			if (plistInfo->count == -1)
			{
				if (asyncStruct->callBack.callbackEnd) asyncStruct->callBack.callbackEnd(-1);
				delete asyncStruct;
				delete plistInfo;
				return;
			}

			if (asyncStruct->callBack.callback)
			{
				asyncStruct->callBack.callback(plistInfo->filename, "", plistInfo->count);
			}
			delete plistInfo;
		}
		else
		{
			--m_asyncRefCount;
			if (0 == m_asyncRefCount)
			{
				Director::getInstance()->getScheduler()->unschedule("refCount", this);
			}

			auto callbackEnd = asyncStruct->callBack.callbackEnd;
			auto retCode = asyncStruct->callBack.retCode;

			delete asyncStruct;
			delete plistInfo;
			if (callbackEnd) callbackEnd(retCode);
			return;
		}
// 		--asyncStruct->ref;
// 		if (asyncStruct->ref == 0)
// 		{
// 			if (asyncStruct->callBack.callbackEnd) asyncStruct->callBack.callbackEnd();
// 
// 			delete asyncStruct;
// 
// 			--m_asyncRefCount;
// 			if (0 == m_asyncRefCount)
// 			{
// 				Director::getInstance()->getScheduler()->unschedule("refCount", this);
// 			}
// 		}
	}

	// the image is generated in loading thread
	std::deque<ImageInfo*> *imagesQueue = m_imageInfoQueue;

	m_imageInfoMutex.lock();
	if (imagesQueue->empty())
	{
		m_imageInfoMutex.unlock();
	}
	else
	{
		ImageInfo *imageInfo = imagesQueue->front();
		imagesQueue->pop_front();

		m_imageInfoMutex.unlock();

		AsyncStruct *asyncStruct = imageInfo->asyncStruct;
		Image *image = imageInfo->image;

		// 保存
		sDat *vDat = nullptr;
		MapDat::iterator iter =  m_mapDat.find(asyncStruct->filename);
		if (iter == m_mapDat.end())
		{
			m_mapDat.insert(MapDat::value_type(asyncStruct->filename, sDat()));
			iter =  m_mapDat.find(asyncStruct->filename);
		}
		vDat = &iter->second;

		// 图片
		if (imageInfo->type == 0)
		{
			vDat->vPng.push_back(imageInfo->filename);

			// tex
			Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(image, imageInfo->filename);

			// SpriteFrames
			m_spriteFramesMutex.lock();
			sSpriteFrameAsync *vSpriteFrame = nullptr;
			size_t infoSize = m_spriteFramesQueue->size();
			for (size_t pos = 0; pos < infoSize; ++pos)
			{
				auto framePlist = (*m_spriteFramesQueue)[pos];
				if (framePlist->nameTex == imageInfo->filename)
				{
					vSpriteFrame = framePlist;
					m_spriteFramesQueue->erase(m_spriteFramesQueue->begin()+pos);
					break;
				}
			}
			m_spriteFramesMutex.unlock();

			if (vSpriteFrame)
			{
				//vDat->vPlist.push_back(vSpriteFrame->name);
				_addSpriteFramesAsync(vSpriteFrame, texture, vDat->vPlist);

				if (asyncStruct->callBack.callback)
				{
					asyncStruct->callBack.callback(vSpriteFrame->name, "", 0);
				}
			}
		}
		// 配置文件
		else if (imageInfo->type == 4)
		{
		}
		// 音效
		else if (imageInfo->type == 1)
		{
			for (auto &v : *imageInfo->valMap)
			{
				vDat->vEffect.push_back(v);
			}
		}
		// 音乐
		else if (imageInfo->type == 2)
		{
			for (auto &v : *imageInfo->valMap)
			{
				vDat->vMusic.push_back(v);
			}
		}

		// call back
		if (asyncStruct->callBack.callback)
		{
			asyncStruct->callBack.callback(imageInfo->filename, imageInfo->datbuf, 0);
		}

		if(image)
		{
			image->release();
		}
		--asyncStruct->ref;
		if (asyncStruct->ref == 0)
		{
			--m_asyncRefCount;
			if (0 == m_asyncRefCount)
			{
				Director::getInstance()->getScheduler()->unschedule("refCount", this);
			}

			if (asyncStruct->callBack.callbackEnd) asyncStruct->callBack.callbackEnd(0);
			delete asyncStruct;
		}
		if (imageInfo->valMap) delete imageInfo->valMap;
		delete imageInfo;
	}
}

void CPreload::_loaddSpriteFrames( sDatHead &datHead, std::string& path, std::string& path2, unsigned char* bytes, int &jps, MapVectString& vPlist, const sCallBack& call )
{
	auto __fileUtils = FileUtils::getInstance();
	auto __spriteFrameCache = SpriteFrameCache::getInstance();
	auto __textureCache = Director::getInstance()->getTextureCache();
	// plist
	int pngsize = *(int*)(bytes+jps); jps += sizeof(int);
	pngsize = decryptionInt(pngsize, datHead.eyCharInt);
	for (int i=0; i<pngsize; ++i)
	{
		int len = *(int*)(bytes+jps); jps += sizeof(int);
		len = decryptionInt(len, datHead.eyCharInt);
		char *strptr = (char*)(bytes+jps); jps += len;
		std::string str = decryptionString(strptr, len, datHead.eyCharStr);

		// plist data
		len = *(int*)(bytes+jps); jps += sizeof(int);
		len = decryptionInt(len, datHead.eyCharInt);
		char* datptr = (char*)(bytes+jps); jps += len;
		std::string datptrdc = decryptionString(datptr, len, datHead.eyCharStr);

		// 加载
		auto dictionary  = __fileUtils->getValueMapFromData(datptrdc.c_str(), len);
		std::string texturePath("");
		if (dictionary.find("metadata") != dictionary.end())
		{
			ValueMap& metadataDict = dictionary["metadata"].asValueMap();
			// try to read  texture file name from meta data
			texturePath = path2 + metadataDict["textureFileName"].asString();
		}
		else
		{
			int idot = str.rfind(".");
			texturePath = path2 + str.substr(0, idot) + ".png";
		}

		ValueMap& framesDict = dictionary["frames"].asValueMap();
		int format = 0;

		// get the format
		if (dictionary.find("metadata") != dictionary.end())
		{
			ValueMap& metadataDict = dictionary["metadata"].asValueMap();
			format = metadataDict["format"].asInt();
		}

		// check the format
		CCASSERT(format >=0 && format <= 3, "format is not supported for SpriteFrameCache addSpriteFramesWithDictionary:textureFilename:");

		// 保存SpriteFrame
		m_spriteFramesDx.insert(MapSpriteFrame::value_type(texturePath, VectSpriteFrame()));
		auto &vSpriteFrame = m_spriteFramesDx.at(texturePath);

		// 记录plist
		vPlist.insert(MapVectString::value_type(path2+str, VectString()));
		auto &vStringPlist = vPlist.at(path2+str);

		cocos2d::Texture2D* texture = nullptr;
		for (auto iter = framesDict.begin(); iter != framesDict.end(); ++iter)
		{
			ValueMap& frameDict = iter->second.asValueMap();
			std::string spriteFrameName = iter->first;

			vStringPlist.push_back(spriteFrameName);

			SpriteFrame* spriteFrame = nullptr;

			if(format == 0) 
			{
				float x = frameDict["x"].asFloat();
				float y = frameDict["y"].asFloat();
				float w = frameDict["width"].asFloat();
				float h = frameDict["height"].asFloat();
				float ox = frameDict["offsetX"].asFloat();
				float oy = frameDict["offsetY"].asFloat();
				int ow = frameDict["originalWidth"].asInt();
				int oh = frameDict["originalHeight"].asInt();
				// check ow/oh
				if(!ow || !oh)
				{
					CCLOGWARN("cocos2d: WARNING: originalWidth/Height not found on the SpriteFrame. AnchorPoint won't work as expected. Regenrate the .plist");
				}
				// abs ow/oh
				ow = abs(ow);
				oh = abs(oh);
				// create frame
				spriteFrame = SpriteFrame::createWithTexture(texture,
					Rect(x, y, w, h),
					false,
					Vec2(ox, oy),
					Size((float)ow, (float)oh)
					);
			} 
			else if(format == 1 || format == 2) 
			{
				Rect frame = RectFromString(frameDict["frame"].asString());
				bool rotated = false;

				// rotation
				if (format == 2)
				{
					rotated = frameDict["rotated"].asBool(); 
				}

				Vec2 offset = PointFromString(frameDict["offset"].asString());
				Size sourceSize = SizeFromString(frameDict["sourceSize"].asString());

				// create frame
				spriteFrame = SpriteFrame::createWithTexture(texture,
					frame,
					rotated,
					offset,
					sourceSize
					);
			} 
			else if (format == 3)
			{
				// get values
				Size spriteSize = SizeFromString(frameDict["spriteSize"].asString());
				Vec2 spriteOffset = PointFromString(frameDict["spriteOffset"].asString());
				Size spriteSourceSize = SizeFromString(frameDict["spriteSourceSize"].asString());
				Rect textureRect = RectFromString(frameDict["textureRect"].asString());
				bool textureRotated = frameDict["textureRotated"].asBool();

				// get aliases

				// create frame
				spriteFrame = SpriteFrame::createWithTexture(texture,
					Rect(textureRect.origin.x, textureRect.origin.y, spriteSize.width, spriteSize.height),
					textureRotated,
					spriteOffset,
					spriteSourceSize);
			}

			// add sprite frame
			__spriteFrameCache->addSpriteFrame(spriteFrame, spriteFrameName);

			// add sprite frame
			sSpriteFrame v;
			v.name = spriteFrameName;
			v.sf = spriteFrame;
			vSpriteFrame.push_back(v);
		}
		// add plist
		__spriteFrameCache->addPlistFileNames(path2+str);

		// 回调通知
		if (call.callback) call.callback(str, "", 0);
	}
}

void CPreload::_loaddSpriteFramesAsync( sDatHead &datHead, AsyncStruct* ast, std::string& path2, unsigned char* bytes, int &jps )
{
	auto __fileUtils = FileUtils::getInstance();
	// plist
	int pngsize = *(int*)(bytes+jps); jps += sizeof(int);
	pngsize = decryptionInt(pngsize, datHead.eyCharInt);
	for (int i=0; i<pngsize; ++i)
	{
		int len = *(int*)(bytes+jps); jps += sizeof(int);
		len = decryptionInt(len, datHead.eyCharInt);
		char *strptr = (char*)(bytes+jps); jps += len;
		std::string str = decryptionString(strptr, len, datHead.eyCharStr);

		// plist data
		len = *(int*)(bytes+jps); jps += sizeof(int);
		len = decryptionInt(len, datHead.eyCharInt);
		char* datptr = (char*)(bytes+jps); jps += len;

		std::string datptrdc = decryptionString(datptr, len, datHead.eyCharStr);

		// 加载
		auto dictionary  = __fileUtils->getValueMapFromData(datptrdc.c_str(), len);
		std::string texturePath("");
		if (dictionary.find("metadata") != dictionary.end())
		{
			ValueMap& metadataDict = dictionary["metadata"].asValueMap();
			// try to read  texture file name from meta data
			texturePath = path2 + metadataDict["textureFileName"].asString();
		}
		else
		{
			int idot = str.rfind(".");
			texturePath = path2 + str.substr(0, idot) + ".png";
		}

		ValueMap& framesDict = dictionary["frames"].asValueMap();
		int format = 0;

		// get the format
		if (dictionary.find("metadata") != dictionary.end())
		{
			ValueMap& metadataDict = dictionary["metadata"].asValueMap();
			format = metadataDict["format"].asInt();
		}

		// check the format
		// CCASSERT(format >=0 && format <= 3, "format is not supported for SpriteFrameCache addSpriteFramesWithDictionary:textureFilename:");
		sSpriteFrameAsync *sfAsy  = new(std::nothrow) sSpriteFrameAsync();
		sfAsy->name = ast->filenamePath + str;
		sfAsy->nameTex = texturePath;
		sfAsy->vMap = new(std::nothrow) VectValueMap();
	
		// Texture2D* texture = nullptr;
		for (auto iter = framesDict.begin(); iter != framesDict.end(); ++iter)
		{
			ValueMap& frameDict = iter->second.asValueMap();
			std::string spriteFrameName = iter->first;

			// vPlist.push_back(spriteFrameName);

			Rect frame;
			bool rotated = false;
			Vec2 offset;
			Size sourceSize;

			// SpriteFrame* spriteFrame = nullptr;
			if(format == 0) 
			{
				frame.origin.x = frameDict["x"].asFloat();
				frame.origin.y = frameDict["y"].asFloat();
				frame.size.width = frameDict["width"].asFloat();
				frame.size.height = frameDict["height"].asFloat();
				offset.x = frameDict["offsetX"].asFloat();
				offset.y = frameDict["offsetY"].asFloat();
				int ow = frameDict["originalWidth"].asInt();
				int oh = frameDict["originalHeight"].asInt();
				// check ow/oh
				if(!ow || !oh)
				{
					CCLOGWARN("cocos2d: WARNING: originalWidth/Height not found on the SpriteFrame. AnchorPoint won't work as expected. Regenrate the .plist");
				}
				// abs ow/oh
				ow = abs(ow);
				oh = abs(oh);

				sourceSize.width = (float)ow;
				sourceSize.height = (float)oh;
				rotated = false;
				// create frame
// 				spriteFrame = SpriteFrame::createWithTexture(texture,
// 					Rect(x, y, w, h),
// 					false,
// 					Vec2(ox, oy),
// 					Size((float)ow, (float)oh)
// 					);
			} 
			else if(format == 1 || format == 2) 
			{
				frame = RectFromString(frameDict["frame"].asString());
				rotated = false;

				// rotation
				if (format == 2)
				{
					rotated = frameDict["rotated"].asBool();
				}

				offset = PointFromString(frameDict["offset"].asString());
				sourceSize = SizeFromString(frameDict["sourceSize"].asString());

				// create frame
// 				spriteFrame = SpriteFrame::createWithTexture(texture,
// 					frame,
// 					rotated,
// 					offset,
// 					sourceSize
// 					);
			} 
			else if (format == 3)
			{
				// get values
				Size spriteSize = SizeFromString(frameDict["spriteSize"].asString());
				offset = PointFromString(frameDict["spriteOffset"].asString());
				sourceSize = SizeFromString(frameDict["spriteSourceSize"].asString());
				frame = RectFromString(frameDict["textureRect"].asString());
				rotated = frameDict["textureRotated"].asBool();

				frame.size = spriteSize;

				// get aliases

				// create frame
// 				spriteFrame = SpriteFrame::createWithTexture(texture,
// 					Rect(textureRect.origin.x, textureRect.origin.y, spriteSize.width, spriteSize.height),
// 					textureRotated,
// 					spriteOffset,
// 					spriteSourceSize);
			}

			// add sprite frame
			sValueMap *vl = new(std::nothrow) sValueMap();
			vl->name = spriteFrameName;
			vl->frame = frame;
			vl->offset = offset;
			vl->rotated = rotated;
			vl->sourceSize = sourceSize;
			sfAsy->vMap->push_back(vl);
		}
		m_spriteFramesMutex.lock();
		m_spriteFramesQueue->push_back(sfAsy);
		m_spriteFramesMutex.unlock();
		// if (call.callback) call.callback(str, 0);

		// 通知回调
// 		sPlistInfo *plist = new(std::nothrow) sPlistInfo();
// 		plist->asyncStruct = ast;
// 		plist->filename = str;
// 		plist->count = 0;
// 
// 		m_plistInfoMutex.lock();
// 		++plist->asyncStruct->ref;
// 		m_plistInfoQueue->push_back(plist);
// 		m_plistInfoMutex.unlock();
	}
}

void CPreload::_addSpriteFramesAsync( sSpriteFrameAsync* sfAsy, cocos2d::Texture2D* texture, MapVectString& vPlist )
{
	auto iter = vPlist.find(sfAsy->name);
	if (iter != vPlist.end())
	{
		return;
	}
	vPlist.insert(MapVectString::value_type(sfAsy->name, VectString()));
	auto &vStringPlist = vPlist.at(sfAsy->name);

	auto __spriteFrameCacheHelper = cocostudio::SpriteFrameCacheHelper::getInstance();

	auto __spriteFrameCache = SpriteFrameCache::getInstance();
	__spriteFrameCache->addPlistFileNames(sfAsy->name);
	for (auto &v : *sfAsy->vMap)
	{
		auto spriteFrame = SpriteFrame::createWithTexture(texture,
			v->frame,
			v->rotated,
			v->offset,
			v->sourceSize
			);

		__spriteFrameCache->addSpriteFrame(spriteFrame, v->name);
		vStringPlist.push_back(v->name);

		__spriteFrameCacheHelper->addSpriteFrames(sfAsy->name, spriteFrame);
		delete v;
	}

	delete sfAsy->vMap;
	delete sfAsy;
}

CPreload::sDatHead CPreload::_readHead( unsigned char* bytes, int &jps )
{
	sDatHead ret;
	int type = *(int*)(bytes+jps); jps += sizeof(int);

	// 跳过数据头
	int ofBytes = *(int*)(bytes+jps); jps += sizeof(int);
	jps += ofBytes;

	// ey
	int eyCharStrvl = *(int*)(bytes+jps); jps += sizeof(int);
	int eyCharIntvl = *(int*)(bytes+jps); jps += sizeof(int);
	ret.eyCharStr = (char)decryptionInt(eyCharStrvl, 't');
	ret.eyCharInt = (char)decryptionInt(eyCharIntvl, 'z');

	// all count
	int count = *(int*)(bytes+jps); jps += sizeof(int);

	// jm
	ret.type = decryptionInt(type, ret.eyCharInt);
	ret.cout = decryptionInt(count, ret.eyCharInt);
	return ret;
}

int CPreload::getPreloadCount( void )
{
	return _sharedCPreload->m_asyncRefCount;
}