#pragma once

#include <string>

#ifdef UNICODE
	typedef std::wstring	tstring;
	typedef wchar_t			tchar;
	#define	TEXT(x)			L##x
	#define _tcslen			wcslen
	#define _tcschr			wcschr
	#define tprintf			wprintf_s
	#define tregex			std::tr1::wregex
	#define tmatch			std::tr1::wcmatch
#else
	#define tregex std::tr1::regex
	#define tmatch std::tr1::cmatch
	typedef std::string		tstring;
	typedef	char			tchar;
	#define	TEXT(x)			x
	#define _tcslen			strlen
	#define _tcschr			strchr
	#define tprintf			sprintf_s
	#define tregex			std::tr1::regex
#endif

typedef unsigned int index_t;
typedef unsigned int flag_t;