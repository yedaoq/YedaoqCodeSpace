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