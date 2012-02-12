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
	if(NULL == section) section = Section_.c_str(); \
	_ASSERT(NULL != section)

HANDLE hModule;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProfile::CProfile()
{
	memset(File_, 0, sizeof(File_));
}

CProfile::CProfile(LPCTSTR lpFile)
{
	if(lpFile != NULL) 
	{
		SetFile(lpFile);
	}
	else
	{
		memset(File_, 0, sizeof(File_));
	}
}

CProfile::~CProfile()
{
}

INT CProfile::GetInt(LPCTSTR key, INT nDefault, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::GetPrivateProfileInt(section, key, nDefault, File_);
}

tstring CProfile::GetString(LPCTSTR key, LPCTSTR lpDefault, LPCTSTR section)
{
	static TCHAR Buffer[MAX_PROFILESTRING_SIZE];
	
	memset(Buffer, 0, MAX_PROFILESTRING_SIZE * sizeof(TCHAR));

	CProfile_PrepareSection;
	
	::GetPrivateProfileString(section, key, lpDefault, Buffer, MAX_PROFILESTRING_SIZE, File_);
	
	return tstring(Buffer);
}

INT CProfile::GetString(LPCTSTR key, LPTSTR Buffer, UINT iBufLen, LPCTSTR lpDefault, LPCTSTR section)
{
	CProfile_PrepareSection;
	
	return ::GetPrivateProfileString(section, key, lpDefault, Buffer, iBufLen, File_);
}

DWORD CProfile::GetSection(LPTSTR lpReturnedString, DWORD nSize, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::GetPrivateProfileSection(section, lpReturnedString, nSize, File_);
}

DWORD CProfile::GetSectionNames(LPTSTR lpReturnedString, DWORD nSize)
{
	return ::GetPrivateProfileSectionNames(lpReturnedString, nSize, File_);
}

BOOL CProfile::GetStruct(LPCTSTR key, LPVOID lpStruct, UINT uSize, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::GetPrivateProfileStruct(section, key, lpStruct, uSize, File_);
}

BOOL CProfile::WriteSection(LPCTSTR lpString, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::WritePrivateProfileSection(section, lpString, File_);
}

BOOL CProfile::WriteString(LPCTSTR key, LPCTSTR lpString, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::WritePrivateProfileString(section, key, lpString, File_);
}

BOOL CProfile::WriteInt(LPCTSTR key, INT iValue, LPCTSTR section)
{
	CProfile_PrepareSection;

	TCHAR BUFFER[8];
	
	memset(BUFFER, 0, sizeof(BUFFER));	
	_tprintf(BUFFER, TEXT("%i"), iValue);
	
	return ::WritePrivateProfileString(section, key, BUFFER, File_);
}

BOOL CProfile::WriteStruct(LPCTSTR key, LPVOID lpStruct, UINT uSize, LPCTSTR section)
{
	CProfile_PrepareSection;

	return ::WritePrivateProfileStruct(section, key, lpStruct, uSize, File_);
}

void CProfile::SetFile(LPCTSTR lpFile)
{
	StrCpyN(File_, lpFile, MAX_PATH - 1);
}

void CProfile::SetCurrentSection(LPCTSTR lpSection)
{
	Section_ = lpSection;
}
