#pragma once

#include <string>

#ifdef UNICODE
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

typedef unsigned int index_t;