/**************************************
 * file - tstring.h
 * auth - yedaoq@gmail.com
 * date - 2012-1-5
 * brief 
 *    micro collection for use the unicode or ansi version of std(or boost) components adaptively
 **************************************/
#pragma once

#include <string>

#ifdef UNICODE

#ifndef TEXT
	#define TEXT(x)			L##x
#endif
	#define tchar			wchar_t
	#define tstring			std::wstring

	#define tboostregex		boost::xpressive::wsregex
	#define tboostcregex	boost::xpressive::wcregex
	#define tboostmatch		boost::xpressive::wsmatch
	#define tboostcmatch	boost::xpressive::wcmatch
	#define tboostformat	boost::wformat

	#define tsregex			std::tr1::wsregex
	#define tsmatch			std::tr1::wsmatch
	#define tcregex			std::tr1::wcregex
	#define tcmatch			std::tr1::wcmatch

	#define tistream		std::wistream
	#define tostream		std::wostream
	#define tstream			std::wiostream

	#define tifstream		std::wifstream
	#define tofstream		std::wofstream
	#define tfstream		std::wfstream

	#define tostringstream	std::wostringstream
	#define tistringstream	std::wistringstream
	#define tstringstream	std::wstringstream

#else

#ifndef 
	#define TEXT(x)			x
#endif

	#define tchar			char
	#define tstring			std::string

	#define tboostregex		boost::xpressive::sregex
	#define tboostcregex	boost::xpressive::cregex
	#define tboostmatch		boost::xpressive::smatch
	#define tboostcmatch	boost::xpressive::cmatch
	#define tboostformat	boost::format

	#define tregex			std::tr1::sregex
	#define tmatch			std::tr1::smatch
	#define tcmatch			std::tr1::cmatch	

	#define tistream		std::istream
	#define tostream		std::ostream
	#define tstream			std::iostream

	#define tifstream		std::ifstream
	#define tofstream		std::ofstream
	#define tfstream		std::fstream

	#define tostringstream	std::ostringstream
	#define tistringstream	std::istringstream
	#define tstringstream	std::stringstream

#endif