#include "stdafx.h"
#include "Helper.h"
#include <objbase.h>
#include <winnt.h>

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
	TRACE(chBuf);
}
tstring StringSplice(IEnumerator<tstring>& source, tstring connecter)
{
	tstring result;
	if(source.MoveNext(result))
	{
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