#ifndef  __TH_UTILITY_H__
#define  __TH_UTILITY_H__

#include "cocos2d.h"
#include "TH_type.h"

namespace TH
{
	// 当前时间
	long getCurrentTime();

	// 获取年月日
	struct stSysTime
	{
		uint16_t year;
		uint16_t month;
		uint16_t day;
		uint16_t hour;
		uint16_t min;
		uint16_t second;
	};
	void getSysTime(stSysTime& gTime);

	// 编码转换
	int codeConvert(const char* from_charset, const char* to_charset, const char* inbuf, size_t inlen, char* outbuf, size_t outlen);

	// UTF8 To GB2312
	std::string u2a(const char* inbuf);

	// GB2312 To UTF8
	std::string a2u(const char* inbuf);

	// 解析
	int analyInt(std::string &str, char type);

	// rand
	void	random_Seed(int seed=0);
	int		random_Int(int min, int max);
	float	random_Float(float min, float max);
}

// 计时器
struct CTHTimer
{
public:
	// 构造函数
	CTHTimer();
	// 开始计时
	void begin();
	// 关闭
	void end();
	// 重新开始
	void reset();
	// 是否开始计时
	bool isBegin() const;
	// 是否过去时间
	bool isPassTime(long time) const;
	// 得到当前时间
	long getCurrentTime() const;
	// 得到开始时间
	long getStartTime() const;
	// 得到流失的时间
	long getLostTime() const;

private:
	long m_dwLastTime;		// 上次时间
	bool  m_bClose;			// 关闭
};

// 数据流
struct CTHByte /* < 2097152(2G) */
{
public:
	// 构造函数
	CTHByte();
	// 析构函数
	virtual ~CTHByte();
	// 根据所给数据创建一个对象
	CTHByte(const uint8_t* pDat, uint32_t size);
	CTHByte(const CTHByte* pByte);
	CTHByte(uint32_t size);
	// 算术重载
	uint8_t& operator[] (unsigned int index);
	CTHByte   operator+  (const CTHByte& thByte);
	CTHByte& operator+= (const CTHByte& thByte);
	CTHByte& operator=  (const CTHByte& thByte);
	// 是否为空
	bool	 isEmpty() const;
	// 清空数据
	void	 clear();
	// 获取长度
	uint32_t getLength() const;
	// 容器大小
	uint32_t size() const;
	// 设置长度
	void	 setLength(uint32_t size);
	void	 resize(uint32_t size);
	// 设置数据
	void	 setData(const void* pDat, uint32_t size);
	void	 setData(const CTHByte& T);
	// 附加数据
	void	 appendData(const void* pDat, uint32_t size);
	template<class _TYPE>
	void	 appendData(const _TYPE* pType);
#ifdef SERVER_OPEN
	// 转网络字节序
	template<class _TYPE>
	void	 appendDataHton(const _TYPE* pType);
#endif

	// 获得数据
	const uint8_t* getData() const;
	uint8_t* getData();
	template<class _TYPE>
	_TYPE*  getData(uint32_t offset = 0) const;

protected:
	// 缓冲数据
	std::vector<uint8_t> m_vcbData;
	uint32_t m_dataof;
	uint32_t m_resize;
};

#include "TH_utility.inl"

#endif // __TH_UTILITY_H__