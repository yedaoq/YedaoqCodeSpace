// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__D164AE02_C5A8_4C5D_893A_71736D45D33F__INCLUDED_)
#define AFX_LOG_H__D164AE02_C5A8_4C5D_893A_71736D45D33F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLog  
{
#define LEVELSTR_SIZE 9
#define BUFFERFORMAT_SIZE 300

public:
	CLog();
	CLog(LPCTSTR lpFile, BOOL bOverWrite = TRUE, UINT iMaxSize = 0);
	virtual ~CLog();
	
	virtual INT SetLogFile(LPCTSTR lpFileName, BOOL bOverWrite = TRUE, UINT iMaxSize = 0);
	virtual BOOL SetLogLevel(INT iExportLevel);
	virtual BOOL Clear();

	virtual BOOL Append(const void *pBuffer, int ilen);
	virtual BOOL AppendIf(BOOL bCondition, const void *pBuffer, int iLen);
	virtual BOOL AppendIf(BOOL bCondition, int level, PCTSTR lpFormat, ...);

protected:
	TCHAR	m_BufferFormat[BUFFERFORMAT_SIZE];
	HANDLE	m_hFilelog;
	UINT	m_iMaxSize;
	UINT	m_iFileSize;
	INT		m_iExportLevel;

	virtual BOOL Close();

public:
	
	static INT LEVEL_DEBUG;
	static INT LEVEL_INFO;
	static INT LEVEL_WARM;
	static INT LEVEL_ERROR;

protected:

	static TCHAR sz_LevelStr[][LEVELSTR_SIZE];
};

#endif // !defined(AFX_LOG_H__D164AE02_C5A8_4C5D_893A_71736D45D33F__INCLUDED_)
