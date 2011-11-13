#pragma once

#ifndef interface
#define interface struct
#endif

#include <string>

#ifdef UNICODE
	#define tregex			boost::xpressive::wsregex
	#define tcregex			boost::xpressive::wcregex
	#define tmatch			boost::xpressive::wsmatch
	#define tcmatch			boost::xpressive::wcmatch
	#define tregex_compiler boost::xpressive::wsregex
	#define tcregex_compiler	boost::xpressive::wcregex
	#define tstringstream	std::wstringstream
	#define tformat			boost::wformat
	typedef std::wstring	tstring;
	typedef wchar_t			tchar;
	#define	TEXT(x)			L##x
	#define _tcslen			wcslen
	#define _tcschr			wcschr
	#define _tcscpy			wcscpy
	#define _tcscmp			wcscmp
	#define tprintf_s		wprintf_s
	#define vstprintf_s		vswprintf_s
	#define tifstream		wifstream
#else
	#define tregex			boost::xpressive::sregex
	#define tcregex			boost::xpressive::cregex
	#define tmatch			boost::xpressive::smatch
	#define tcmatch			boost::xpressive::cmatch
	#define tregex_compiler boost::xpressive::sregex
	#define tcregex_compiler	boost::xpressive::cregex
	#define tformat			boost::format
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
	#define tifstream		ifstream
#endif

typedef unsigned int index_t;
typedef unsigned int flag_t;