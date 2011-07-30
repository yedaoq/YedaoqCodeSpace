#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>

void Helper_Log(char * pszFormat,...)
{
	va_list args;
	char buf[1024];
	int nBytesWritten = 0;
	
	if( NULL == pszFormat)
	{
		return;
	}
	
	va_start(args, pszFormat);
	_vsnprintf(buf, sizeof(buf) - 1, pszFormat, args);
	va_end(args);
	
	//WriteFile(g_hLog, buf, lstrlen(buf), &nBytesWritten, NULL);
}

void CallLog(int iArg1, int iArg2)
{
	Helper_Log("Test %d %d", iArg1, iArg2);
	
	int iTemp = 0;
}

void mainVariableArgument()
{
	CallLog(1,1);
}