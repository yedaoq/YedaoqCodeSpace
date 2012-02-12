// Profile.h: interface for the CProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILE_H__F48B8CE6_3463_4459_9234_90F424AB7319__INCLUDED_)
#define AFX_PROFILE_H__F48B8CE6_3463_4459_9234_90F424AB7319__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "tstring.h"



extern HANDLE hModule;

class CProfile  
{
#define MAX_PROFILESTRING_SIZE 256
#define MAX_SECTIONNAME_SIZE 50

public:

	CProfile();
	CProfile(LPCTSTR strFile);
	virtual ~CProfile();

public:

	void		SetFile(LPCTSTR lpFile);
	LPCTSTR		GetFileName() const { return File_; };

	void		SetCurrentSection(LPCTSTR lpSection);
	const tstring&	GetCurrentSection() const { return Section_; };

	BOOL		GetStruct(LPCTSTR key, LPVOID lpStruct, UINT uSize, LPCTSTR section = NULL);
	DWORD		GetSectionNames(LPTSTR lpReturnedString, DWORD nSize);
	DWORD		GetSection(LPTSTR lpReturnedString, DWORD nSize, LPCTSTR section = NULL);
	tstring		GetString(LPCTSTR key, LPCTSTR lpDefault = NULL, LPCTSTR section = NULL);
	INT			GetString(LPCTSTR key, LPTSTR Buffer, UINT iBufLen, LPCTSTR lpDefault = NULL, LPCTSTR section = NULL);
	INT			GetInt(LPCTSTR key, INT nDefault = 0, LPCTSTR section = NULL);
	
	BOOL		WriteStruct(LPCTSTR key, LPVOID lpStruct, UINT uSize, LPCTSTR section = NULL);
	BOOL		WriteInt(LPCTSTR key, INT iValue, LPCTSTR section = NULL);
	BOOL		WriteString(LPCTSTR key, LPCTSTR lpString, LPCTSTR section = NULL);
	BOOL		WriteSection(LPCTSTR lpString, LPCTSTR section = NULL);

protected:

	tstring		Section_;
	tchar		File_[MAX_PATH];
};

#endif // !defined(AFX_PROFILE_H__F48B8CE6_3463_4459_9234_90F424AB7319__INCLUDED_)
