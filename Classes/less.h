//
// •¶š—ñ”äŠrƒNƒ‰ƒX(‘å•¶š^¬•¶š‚ğ‹æ•Ê‚µ‚È‚¢)
//
//		Copyright (c) 2000-2001 Chihiro.SAKAMOTO (HyperWorks)
//
#ifndef	__less_h__
#define	__less_h__
#include <string.h> /* strcasecmp (4.4BSD - compatibility), _stricmp (_WIN32) */

class ic_less: public binary_function<const char *, const char *, bool> {
  public:
	result_type operator()(first_argument_type x, second_argument_type y) const
	{
#if defined(_WIN32)
		return _stricmp(x, y)<0;
#else
		return strcasecmp( x, y )<0;
#endif
		//return strcasecmp(x, y) < 0;
	}
} ;

class ics_less: public binary_function<const string &, const string &, bool> {
  public:
	result_type operator()(first_argument_type x, second_argument_type y) const
	{
#if defined(_WIN32)
		return _stricmp(x.c_str(), y.c_str()) < 0;
#else
		return strcasecmp(x.c_str(), y.c_str()) < 0;
#endif
		//return strcasecmp(x.c_str(), y.c_str()) < 0;
	}
} ;

#endif
