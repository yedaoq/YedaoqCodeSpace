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

	#define tregex			std::tr1::wsregex
	#define tmatch			std::tr1::wsmatch
	#define tcmatch			std::tr1::wcmatch

	#define tistream		wistream
	#define tostream		wostream
	#define tstream			wiostream

	#define tifstream		wifstream
	#define tofstream		wofstream
	#define tfstream		wfstream

	#define tostringstream	wostringstream
	#define tistringstream	wistringstream
	#define tstringstream	wstringstream

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

	#define tistream		istream
	#define tostream		ostream
	#define tstream			iostream

	#define tifstream		ifstream
	#define tofstream		ofstream
	#define tfstream		fstream

	#define tostringstream	ostringstream
	#define tistringstream	istringstream
	#define tstringstream	stringstream

#endif