//////////////////////////////////////////////////////////////////////////

//namespace TH
//{
//	
//}

//////////////////////////////////////////////////////////////////////////
// CTHTimer
inline CTHTimer::CTHTimer()
	:m_dwLastTime(0)
	,m_bClose(true)
{ }

inline void CTHTimer::begin()
{
	if (m_bClose)
	{
		// Began to record the time
		reset();
		m_bClose = false;
	}
}

inline void CTHTimer::end()
{
	// end time
	m_bClose = true;
	m_dwLastTime = 0;
}

inline void CTHTimer::reset()
{
	// Reset the time
	m_dwLastTime = TH::getCurrentTime();
}

inline bool CTHTimer::isBegin() const
{
	return !m_bClose;
}

inline bool CTHTimer::isPassTime( long time ) const
{
	// Will the past time
	return m_bClose ? false : (getLostTime() >= time);
}

inline long CTHTimer::getCurrentTime() const
{
	// Will the past time
	return TH::getCurrentTime();
}

inline long CTHTimer::getStartTime() const
{
	return m_dwLastTime;
}

inline long CTHTimer::getLostTime() const
{
	return TH::getCurrentTime() - m_dwLastTime;
}

//////////////////////////////////////////////////////////////////////////
// CTHByte

inline CTHByte::CTHByte()
	: m_dataof(0)
	, m_resize(0)
{
}

inline CTHByte::CTHByte( const uint8_t *pDat , uint32_t Size )
	: m_dataof(0)
	, m_resize(0)
{
	setData(pDat, Size);
}

inline CTHByte::CTHByte( const CTHByte *pByte )
	: m_dataof(0)
	, m_resize(0)
{
	setData(pByte);
}

inline CTHByte::CTHByte( uint32_t Size )
	: m_dataof(0)
	, m_resize(0)
{
	CCAssert(Size > 0, "");
	setLength(Size);
}

inline CTHByte::~CTHByte()
{
	m_dataof = 0;
	m_resize = 0;
	m_vcbData.resize(0);
	m_vcbData.clear();
}

// According to the data to create an object
inline uint8_t & CTHByte::operator[]( unsigned int Index )
{
	CCAssert(Index < size(), "");
	CCAssert(Index >= 0, "");
	return m_vcbData[Index];
}

inline CTHByte CTHByte::operator+( const CTHByte &thByte )
{
	CTHByte T(*this);
	T += thByte;
	return T;
}

inline CTHByte & CTHByte::operator+=( const CTHByte &thByte )
{
	CCAssert(!thByte.isEmpty(), "");
	uint32_t size = thByte.getLength();
	CTHByte tempByte(thByte);
	appendData(tempByte.getData(), size);
	return *this;
}

inline CTHByte & CTHByte::operator=( const CTHByte &thByte )
{
	CCAssert(!thByte.isEmpty(), "");
	m_vcbData = thByte.m_vcbData;
	m_dataof = thByte.m_dataof;
	return *this;
}

inline bool CTHByte::isEmpty() const
{
	// For null
	return m_dataof == 0;
}

inline void CTHByte::clear()
{
	m_dataof = 0;
}

inline uint32_t CTHByte::getLength() const
{
	// Obtains the data length
	return m_dataof*sizeof(uint8_t);
}

inline uint32_t CTHByte::size() const
{
	return m_dataof;
}

inline void CTHByte::setLength( uint32_t Size )
{
	CCAssert(Size >= 0, "");
	if (Size <= m_resize) return;
	resize(Size);
}

inline void CTHByte::resize( uint32_t Size )
{
	CCAssert(Size >= 0, "");
	if (m_vcbData.size() == Size) return;
	m_vcbData.resize(Size);
	//m_dataof = 0;
	m_resize = Size;
}

inline void CTHByte::setData( const void *pDat , uint32_t Size )
{
	CCAssert(Size > 0, ""); /* length > 0 */
	CCAssert(pDat != NULL, "");
	setLength(Size);
	memcpy(&m_vcbData[0], pDat, Size);
	m_dataof = Size;
}

inline void CTHByte::setData( const CTHByte &T )
{
	CCAssert(T.size() > 0, "");
	m_vcbData = T.m_vcbData;
	m_dataof = T.m_dataof;
	m_resize = T.m_resize;
}

inline void CTHByte::appendData( const void *pDat , uint32_t Size )
{
	CCAssert(Size > 0, "");
	CCAssert(pDat != NULL, "");
	// Size Try to less than < 2097152(2G)
	{
		uint32_t OldSize = size();
		uint32_t NewSize = OldSize + Size;
		setLength(NewSize);
		memcpy(&m_vcbData[OldSize], pDat, Size);
		m_dataof += Size;
	}
}

template<class _TYPE>
inline void CTHByte::appendData( const _TYPE *pType )
{
	CCAssert(pType != NULL, "");
	appendData(pType , sizeof(_TYPE));
}

#ifdef SERVER_OPEN
template<class _TYPE>
inline void CTHByte::appendDataHton( const _TYPE *pType )
{
	CCAssert(pType != NULL, "");
	_TYPE _val = *pType;
	if (sizeof(_TYPE) == sizeof(uint16))
	{
		_val = htons(_val);
	}
	else if (sizeof(_TYPE) == sizeof(uint32))
	{
		_val = htonl(_val);
	}
	else
	{
		_val = _val;
	}
	appendData(&_val , sizeof(_TYPE));
}
#endif

inline const uint8_t* CTHByte::getData() const
{
	if(!isEmpty())
		return &m_vcbData[0];
	return NULL;
}

inline uint8_t* CTHByte::getData()
{
	if (isEmpty())
	{
		return NULL;
	}
	return &m_vcbData[0];
}

template<class _TYPE>
inline _TYPE* CTHByte::getData( uint32_t Offset /*= 0 */ ) const
{
	if((Offset + sizeof(_TYPE)) > getLength()) /* data offset */
		return NULL;
	return (_TYPE*)(getData() + Offset);
}