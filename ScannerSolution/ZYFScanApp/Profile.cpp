// Profile.cpp: implementation of the CProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Profile.h"
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CProfile_PrepareSection \
	if(NULL == section) section = m_strSection; \
	ASSERT(NULL != section)

HANDLE hModule;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProfile::CProfile()
{
	memset(m_strSection, 0, sizeof(m_strSection));
	memset(m_strFile, 0, sizeof(m_strFile));
}

CProfile::CProfile(LPCTSTR lpFile)
{
	if(lpFile != NULL) 
	{
		SetCurrentFile(lpFile);
	}
	else
	{
		memset(m_strFile, 0, sizeof(m_strFile));
	}
	
	memset(m_strSection, 0, sizeof(m_strSection));
}

CProfile::~CProfile()
{
}

INT CProfile::GetInt(LPCTSTR key, INT nDefault, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::GetPrivateProfileInt(section, key, nDefault, m_strFile);
}

CString CProfile::GetString(LPCTSTR key, LPCTSTR lpDefault, LPCTSTR section)
{
	static TCHAR Buffer[MAX_PROFILESTRING_SIZE];
	
	memset(Buffer, 0, MAX_PROFILESTRING_SIZE * sizeof(TCHAR));

	CProfile_PrepareSection;
	
	::GetPrivateProfileString(section, key, lpDefault, Buffer, MAX_PROFILESTRING_SIZE, m_strFile);
	
	return CString(Buffer);
}

INT CProfile::GetString(LPCTSTR key, LPTSTR Buffer, UINT iBufLen, LPCTSTR lpDefault, LPCTSTR section)
{
	CProfile_PrepareSection;
	
	return ::GetPrivateProfileString(section, key, lpDefault, Buffer, iBufLen, m_strFile);
}

DWORD CProfile::GetSection(LPTSTR lpReturnedString, DWORD nSize, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::GetPrivateProfileSection(section, lpReturnedString, nSize, m_strFile);
}

DWORD CProfile::GetSectionNames(LPTSTR lpReturnedString, DWORD nSize)
{
	return ::GetPrivateProfileSectionNames(lpReturnedString, nSize, m_strFile);
}

BOOL CProfile::GetStruct(LPCTSTR key, LPVOID lpStruct, UINT uSize, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::GetPrivateProfileStruct(section, key, lpStruct, uSize, m_strFile);
}

BOOL CProfile::WriteSection(LPCTSTR lpString, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::WritePrivateProfileSection(section, lpString, m_strFile);
}

BOOL CProfile::WriteString(LPCTSTR key, LPCTSTR lpString, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::WritePrivateProfileString(section, key, lpString, m_strFile);
}

BOOL CProfile::WriteInt(LPCTSTR key, INT iValue, LPCTSTR section)
{
	CProfile_PrepareSection;

	TCHAR BUFFER[8];
	
	memset(BUFFER, 0, sizeof(BUFFER));
	
	_tprintf(BUFFER, TEXT("%i"), iValue);
	
	return ::WritePrivateProfileString(section, key, BUFFER, m_strFile);
}

BOOL CProfile::WriteStruct(LPCTSTR key, LPVOID lpStruct, UINT uSize, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::WritePrivateProfileStruct(section, key, lpStruct, uSize, m_strFile);
}

void CProfile::SetCurrentFile(LPCTSTR lpFile)
{
	StrCpyN(m_strFile, lpFile, MAX_PATH - 1);
}

void CProfile::SetCurrentSection(LPCTSTR lpSection)
{
	StrCpyN(m_strSection, lpSection, MAX_SECTIONNAME_SIZE - 1);
}

LPCTSTR CProfile::GetCurrentSection()
{
	return m_strSection;
}
LPCTSTR CProfile::GetCurrentFile()
{
	return m_strFile;
}
