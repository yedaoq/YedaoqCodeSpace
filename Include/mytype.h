#pragma once

#ifndef interface
#define interface struct
#endif

#include <string>

#ifdef UNICODE
	#define tregex			std::tr1::wregex
	#define tmatch			std::tr1::wsmatch
	#define tcmatch			std::tr1::wcmatch
	#define tstringstream	std::wstringstream
	typedef std::wstring	tstring;
	typedef wchar_t			tchar;
	#define	TEXT(x)			L##x
	#define _tcslen			wcslen
	#define _tcschr			wcschr
	#define _tcscpy			wcscpy
	#define _tcscmp			wcscmp
	#define tprintf_s		wprintf_s
	#define vstprintf_s		vswprintf_s
#else
	#define tregex			std::tr1::regex
	#define tmatch			std::tr1::smatch
	#define tcmatch			std::tr1::cmatch
	#define tstringstream	std::stringstream
	typedef std::string		tstring;
	typedef	char			tchar;
	#define	TEXT(x)			x
	#define _tcslen			strlen
	#define _tcschr			strchr
	#define _tcscpy			strcpy
	#define _tcscmp			strcmp
	#define tprintf_s		sprintf_s
	#define vstprintf_s		vsprintf_s
#endif

typedef unsigned int index_t;
typedef unsigned int flag_t;