#include "stdafx.h"
#include "Helper.h"
#include <objbase.h>
#include <winnt.h>
#include "boost\format.hpp"
#include "boost\xpressive\xpressive.hpp"
#include "boost\lexical_cast.hpp"
#include <boost\xpressive\xpressive_fwd.hpp>

EnumMessageBoxResult MsgboxPrompt(LPCTSTR format, ...)
{
	TCHAR buffer[200];
	va_list args;
	va_start(args, format);
	_vstprintf_s(buffer, sizeof(buffer)/sizeof(buffer[0]), format, args );
	return (EnumMessageBoxResult)MessageBox(NULL, buffer, TEXT("提示"), MB_OK | MB_ICONINFORMATION);
}

EnumMessageBoxResult MsgboxConfirm(LPCTSTR format, ...)
{
	TCHAR buffer[200];
	va_list args;
	va_start(args, format);
	_vstprintf_s(buffer, sizeof(buffer)/sizeof(buffer[0]), format, args );
	return (EnumMessageBoxResult)MessageBox(NULL, buffer, TEXT("确认"), MB_OKCANCEL | MB_ICONQUESTION);
}

EnumMessageBoxResult MsgboxError(LPCTSTR format, ...)
{
	TCHAR buffer[200];
	va_list args;
	va_start(args, format);
	_vstprintf_s(buffer, sizeof(buffer)/sizeof(buffer[0]), format, args );
	return (EnumMessageBoxResult)MessageBox(NULL, buffer, TEXT("错误"), MB_OK | MB_ICONERROR);
}

void WTRACE(wchar_t const* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	wchar_t szBuf[1024];
	ATL_CRT_ERRORCHECK_SPRINTF(_vsnwprintf_s(szBuf, _countof(szBuf), _countof(szBuf) - 1, pszFmt, args));
	va_end(args);

	char chBuf[1024];
	WideCharToMultiByte(CP_ACP, 0, szBuf, -1, chBuf, sizeof(chBuf), NULL, NULL);
	OutputDebugStringA(chBuf);
}
tstring StringSplice(IEnumerator<tstring>& source, tstring connecter)
{
	tstring result;
	if(source.MoveNext())
	{
		result = source.Current();
		while(source.MoveNext())
		{
			result += connecter;
			result += source.Current();
		}
	}

	return result;
}

tstring ToStr(const int& val)
{
	TCHAR buf[20];
	_itot(val, buf, 10);
	return buf;
}

tstring ToStr(const GUID& guid)
{
	return GUIDToStr(guid);
}

tstring ToStr(const CPoint& pt)
{
	tformat fmt(TEXT("%d, %d"));
	return (fmt%pt.x%pt.y).str();
}

tstring ToStr(const CSize& sz)
{
	tformat fmt(TEXT("%d, %d"));
	return (fmt%sz.cx%sz.cy).str();
}

template<typename T> 
T FromStr(const tstring& str)
{
	return T;
}
template<> CPoint FromStr<CPoint>(const tstring& str)
{
	CPoint pt;
	if(str.empty()) return pt;

	tmatch mth;
	tregex reg = boost::xpressive::wsregex_compiler().compile(TEXT("\\s*\\s*(\\d+)\\s*,\\s*(\\d+)\\s*"));
	if(boost::xpressive::regex_match(str, mth, reg))
	{
		pt.x = boost::lexical_cast<long>(mth[1].str());
		pt.y = boost::lexical_cast<long>(mth[2].str());
	}

	return pt;
}

template<> CSize FromStr<CSize>(const tstring& str)
{
	CSize pt;
	if(str.empty()) return pt;

	tmatch mth;
	tregex reg = boost::xpressive::wsregex_compiler().compile(TEXT("\\s*\\s*(\\d+)\\s*,\\s*(\\d+)\\s*"));
	if(boost::xpressive::regex_match(str, mth, reg))
	{
		pt.cx = boost::lexical_cast<int>(mth[1].str());
		pt.cy = boost::lexical_cast<int>(mth[2].str());
	}

	return pt;
}

tstring GUIDToStr(GUID clsid)
{
	LPOLESTR str = NULL;
	StringFromCLSID(clsid, &str);
	tstring ret(str);
	CoTaskMemFree(str);
	return ret;
}

GUID StrToGUID(tstring str)
{
	GUID ret;
	CLSIDFromString((LPOLESTR)str.c_str(),&ret);
	return ret;
}