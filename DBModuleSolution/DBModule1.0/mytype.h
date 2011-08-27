#pragma once

#include <string>

#ifdef UNICODE
	typedef std::wstring	tstring;
	typedef wchar_t			tchar;
#else
	typedef std::string		tstring;
	typedef	char			tchar;
#endif

typedef unsigned int index_t;
typedef unsigned int flag_t;