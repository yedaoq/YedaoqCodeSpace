#pragma once

#include <string>

#ifdef UNICODE
	typedef std::wstring	tstring;
	typedef wchar_t			tchar;
	#define	TEXT(x)			L##x
	#define _tcslen			wcslen
	#define _tcschr			wcschr
#else
	typedef std::string		tstring;
	typedef	char			tchar;
	#define	TEXT(x)			x
	#define _tcslen			strlen
	#define _tcschr			strchr
#endif

typedef unsigned int index_t;
typedef unsigned int flag_t;