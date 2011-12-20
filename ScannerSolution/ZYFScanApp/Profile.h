// Profile.h: interface for the CProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILE_H__F48B8CE6_3463_4459_9234_90F424AB7319__INCLUDED_)
#define AFX_PROFILE_H__F48B8CE6_3463_4459_9234_90F424AB7319__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



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

	BOOL GetStruct(LPCTSTR key, LPVOID lpStruct, UINT uSize, LPCTSTR section = NULL);
	DWORD GetSectionNames(LPTSTR lpReturnedString, DWORD nSize);
	DWORD GetSection(LPTSTR lpReturnedString, DWORD nSize, LPCTSTR section = NULL);
	CString GetString(LPCTSTR key, LPCTSTR lpDefault = NULL, LPCTSTR section = NULL);
	INT GetString(LPCTSTR key, LPTSTR Buffer, UINT iBufLen, LPCTSTR lpDefault = NULL, LPCTSTR section = NULL);
	INT GetInt(LPCTSTR key, INT nDefault = 0, LPCTSTR section = NULL);
	
	BOOL WriteStruct(LPCTSTR key, LPVOID lpStruct, UINT uSize, LPCTSTR section = NULL);
	BOOL WriteInt(LPCTSTR key, INT iValue, LPCTSTR section = NULL);
	BOOL WriteString(LPCTSTR key, LPCTSTR lpString, LPCTSTR section = NULL);
	BOOL WriteSection(LPCTSTR lpString, LPCTSTR section = NULL);

	void SetCurrentSection(LPCTSTR lpSection);
	void SetCurrentFile(LPCTSTR lpFile);

	LPCTSTR GetCurrentSection();
	LPCTSTR GetCurrentFile();

protected:

	TCHAR m_strSection[MAX_SECTIONNAME_SIZE];
	TCHAR m_strFile[MAX_PATH];
};

#endif // !defined(AFX_PROFILE_H__F48B8CE6_3463_4459_9234_90F424AB7319__INCLUDED_)
