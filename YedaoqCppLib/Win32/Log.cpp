// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Log.h"
#include "ErrorCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

INT CLog::LEVEL_DEBUG = 1;
INT CLog::LEVEL_INFO = 2;
INT CLog::LEVEL_WARM = 3;
INT CLog::LEVEL_ERROR = 4;

TCHAR CLog::sz_LevelStr[][LEVELSTR_SIZE] = {TEXT("NONE  : "), TEXT("DEBUG : "), TEXT("INFO  : "), TEXT("WARM  : "), TEXT("ERROR : ")};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog(LPCTSTR lpFile, BOOL bOverWrite, UINT iMaxSize) : m_hFilelog(INVALID_HANDLE_VALUE), m_iMaxSize(0), m_iFileSize(0)
{
	SetLogFile(lpFile, bOverWrite, iMaxSize);
}

CLog::CLog(void) : m_hFilelog(INVALID_HANDLE_VALUE), m_iMaxSize(0), m_iFileSize(0)
{
	
}

CLog::~CLog()
{
	if(INVALID_HANDLE_VALUE != m_hFilelog)
	{
		Close();
	}
}

BOOL CLog::Close()
{
	if(INVALID_HANDLE_VALUE != m_hFilelog) return TRUE;

	try
	{
		FlushFileBuffers(m_hFilelog);
	}
	catch(...){}

	try
	{
		CloseHandle(m_hFilelog);
	}
	catch(...){}
	
	m_hFilelog = INVALID_HANDLE_VALUE;
	
	return TRUE;
}

BOOL CLog::SetLogFile(LPCTSTR lpFileName, BOOL bOverWrite, UINT iMaxSize)
{
	if(NULL == lpFileName)
	{
		return MYERROR_INVALID_PARA;
	}

	Close();

	try
	{
		m_hFilelog = CreateFile(lpFileName, 
			GENERIC_WRITE, 
			FILE_SHARE_READ, 
			NULL, 
			CREATE_NEW | (bOverWrite ? CREATE_ALWAYS : OPEN_EXISTING), 
			FILE_ATTRIBUTE_NORMAL, 
			INVALID_HANDLE_VALUE);
	}
	catch (...){}

	this->m_iMaxSize = iMaxSize;
	if(!bOverWrite) m_iFileSize = GetFileSize(m_hFilelog, NULL);

	return TRUE;
}

BOOL CLog::SetLogLevel(INT iExportLevel)
{
	m_iExportLevel = iExportLevel;
	return TRUE;
}

BOOL CLog::Clear()
{
	BOOL bResult = FALSE;

	if(INVALID_HANDLE_VALUE == m_hFilelog) return FALSE;
	
	try{
		SetFilePointer(m_hFilelog, 0, 0, FILE_BEGIN);
		SetEndOfFile(m_hFilelog);
		m_iFileSize = 0;

		bResult = TRUE;
	}
	catch(...){}

	return bResult;
}

BOOL CLog::Append(const void *buffer, int ilen)
{
	DWORD dwBytesWriten = 0;
	BOOL bResult = FALSE;

	if(NULL == buffer || ilen <= 0) return FALSE;
	if(INVALID_HANDLE_VALUE == m_hFilelog) return FALSE;

	if(m_iMaxSize > 0 && m_iFileSize + ilen >= m_iMaxSize)
	{
		if(!Clear()) return FALSE;
	}
	
	try
	{
		bResult = WriteFile(m_hFilelog, buffer, ilen, &dwBytesWriten, NULL);
		m_iFileSize += dwBytesWriten;
	}
	catch (...){}
	
	return bResult;
}

BOOL CLog::AppendIf(BOOL bCondition, const void *buffer, int iLen)
{
	if(bCondition) 
	{
		return Append(buffer,iLen);
	}

	return FALSE;
}

BOOL CLog::AppendIf(BOOL bCondition, int level, PCTSTR lpFormat, ...)
{
	va_list args;
	PTCHAR buffer = m_BufferFormat;
	int nSize = 0;
	TCHAR lpNewLine[] = TEXT("\r\n");

	if(!bCondition) return TRUE;
	if(level < m_iExportLevel) return TRUE;
	if(level < LEVEL_DEBUG || level > LEVEL_ERROR || NULL == lpFormat) return FALSE;
	
	lstrcpy(buffer, sz_LevelStr[level]);
	buffer += LEVELSTR_SIZE - 1;
	
	try
	{
		va_start(args,lpFormat);		
		nSize = _vsntprintf(buffer, BUFFERFORMAT_SIZE - LEVELSTR_SIZE - lstrlen(lpNewLine), lpFormat, args);
	}
	catch (...)	
	{
		return FALSE;
	}

	if(nSize < 0) return FALSE;

	buffer += nSize;
	lstrcpy(buffer, lpNewLine);
	
	return Append(m_BufferFormat, lstrlen(m_BufferFormat) * sizeof(TCHAR));
}



