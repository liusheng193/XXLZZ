#ifndef  __TH_UTILITY_H__
#define  __TH_UTILITY_H__

#include "cocos2d.h"
#include "TH_type.h"

namespace TH
{
	// ��ǰʱ��
	long getCurrentTime();

	// ��ȡ������
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

	// ����ת��
	int codeConvert(const char* from_charset, const char* to_charset, const char* inbuf, size_t inlen, char* outbuf, size_t outlen);

	// UTF8 To GB2312
	std::string u2a(const char* inbuf);

	// GB2312 To UTF8
	std::string a2u(const char* inbuf);

	// ����
	int analyInt(std::string &str, char type);

	// rand
	void	random_Seed(int seed=0);
	int		random_Int(int min, int max);
	float	random_Float(float min, float max);
}

// ��ʱ��
struct CTHTimer
{
public:
	// ���캯��
	CTHTimer();
	// ��ʼ��ʱ
	void begin();
	// �ر�
	void end();
	// ���¿�ʼ
	void reset();
	// �Ƿ�ʼ��ʱ
	bool isBegin() const;
	// �Ƿ��ȥʱ��
	bool isPassTime(long time) const;
	// �õ���ǰʱ��
	long getCurrentTime() const;
	// �õ���ʼʱ��
	long getStartTime() const;
	// �õ���ʧ��ʱ��
	long getLostTime() const;

private:
	long m_dwLastTime;		// �ϴ�ʱ��
	bool  m_bClose;			// �ر�
};

// ������
struct CTHByte /* < 2097152(2G) */
{
public:
	// ���캯��
	CTHByte();
	// ��������
	virtual ~CTHByte();
	// �����������ݴ���һ������
	CTHByte(const uint8_t* pDat, uint32_t size);
	CTHByte(const CTHByte* pByte);
	CTHByte(uint32_t size);
	// ��������
	uint8_t& operator[] (unsigned int index);
	CTHByte   operator+  (const CTHByte& thByte);
	CTHByte& operator+= (const CTHByte& thByte);
	CTHByte& operator=  (const CTHByte& thByte);
	// �Ƿ�Ϊ��
	bool	 isEmpty() const;
	// �������
	void	 clear();
	// ��ȡ����
	uint32_t getLength() const;
	// ������С
	uint32_t size() const;
	// ���ó���
	void	 setLength(uint32_t size);
	void	 resize(uint32_t size);
	// ��������
	void	 setData(const void* pDat, uint32_t size);
	void	 setData(const CTHByte& T);
	// ��������
	void	 appendData(const void* pDat, uint32_t size);
	template<class _TYPE>
	void	 appendData(const _TYPE* pType);
#ifdef SERVER_OPEN
	// ת�����ֽ���
	template<class _TYPE>
	void	 appendDataHton(const _TYPE* pType);
#endif

	// �������
	const uint8_t* getData() const;
	uint8_t* getData();
	template<class _TYPE>
	_TYPE*  getData(uint32_t offset = 0) const;

protected:
	// ��������
	std::vector<uint8_t> m_vcbData;
	uint32_t m_dataof;
	uint32_t m_resize;
};

#include "TH_utility.inl"

#endif // __TH_UTILITY_H__