#pragma once

#include <string>

#ifdef UNICODE
	#define tregex			std::tr1::wregex
	#define tmatch			std::tr1::wcmatch
	typedef std::wstring	tstring;
	typedef wchar_t			tchar;
	#define	TEXT(x)			L##x
	#define _tcslen			wcslen
	#define _tcschr			wcschr
	#define _tcscpy			wcscpy
	#define tprintf_s		wprintf_s
	#define vstprintf_s		vswprintf_s
#else
	#define tregex			std::tr1::regex
	#define tmatch			std::tr1::cmatch
	typedef std::string		tstring;
	typedef	char			tchar;
	#define	TEXT(x)			x
	#define _tcslen			strlen
	#define _tcschr			strchr
	#define _tcscpy			strcpy
	#define tprintf_s		sprintf_s
	#define vstprintf_s		vsprintf_s
#endif

typedef unsigned int index_t;
typedef unsigned int flag_t;