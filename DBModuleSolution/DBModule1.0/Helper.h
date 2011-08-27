#pragma once
#include <tchar.h>
#include "mytype.h"
#include "Enumerator.h"
#include <winuser.h>

void WTRACE(wchar_t const* pszFmt, ...);

#ifdef UNICODE
#define TTRACE WTRACE
#else
#define TTRACE TRACE
#endif

struct GUID;

enum EnumMessageBoxResult
{
	MBRABORT = IDABORT, //Abort button was selected. 
	MBRCANCEL = IDCANCEL,// Cancel button or the close button on the title bar was selected. 
	MBRIGNORE = IDIGNORE,// Ignore button was selected. 
	MBRNO = IDNO,// No button was selected. 
	MBROK = IDOK,// OK button was selected. 
	MBRRETRY = IDRETRY,// Retry button was selected. 
	MBRYES = IDYES //Yes button was selected. 
};

EnumMessageBoxResult MsgboxPrompt(LPCTSTR format, ...);
EnumMessageBoxResult MsgboxError(LPCTSTR format, ...);
EnumMessageBoxResult MsgboxConfirm(LPCTSTR format, ...);

tstring ToStr(const int& val);
tstring ToStr(const GUID& guid);

tstring GUIDToStr(GUID clsid);
GUID StrToGUID(tstring str);

tstring StringSplice(IEnumerator<tstring>& source, tstring connecter);

enum EnumTraversalControl{Normal = 0, Stop, IgnoreChild };