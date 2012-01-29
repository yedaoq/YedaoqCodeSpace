/* ___________________________________________________________________________
@ 
@ file - DirectoryEnumerator.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-29
@ info -
@     文件目录枚举器
/* ___________________________________________________________________________*/

#pragma once

#include <winbase.h>
#include "..\CPP\Enumerator.h"
#include <string>
#include "..\cpp\tstring.h"

class CDirectoryEnumerator : CEnumeratorBase<WIN32_FIND_DATA>
{
public:
	~CDirectoryEnumerator()
	{
		if (INVALID_HANDLE_VALUE != m_FindHandle)
		{
			Close();
		}
	}

	CDirectoryEnumerator()
		: m_Dir(TEXT(""))
		, m_NamePattern(TEXT("*"))
		, m_FileDataValidaty(false)
		, m_FindHandle(INVALID_HANDLE_VALUE)
	{}

	CDirectoryEnumerator(LPCTSTR lpDir, LPCTSTR lpNamePattern = TEXT("*"))
		: m_NamePattern(lpNamePattern)
		, m_FileDataValidaty(false)
		, m_FindHandle(INVALID_HANDLE_VALUE)
	{
		SetDir(lpDir);
	}

	CDirectoryEnumerator(const CDirectoryEnumerator& other)
		: m_Dir(other.m_Dir)
		, m_NamePattern(other.m_NamePattern)
		, m_FileDataValidaty(false)
		, m_FindHandle(INVALID_HANDLE_VALUE)
	{}

	CDirectoryEnumerator& operator=(const CDirectoryEnumerator& other)
	{
		if(this != &other)
		{
			Close();
			m_Dir = other.m_Dir;
			m_NamePattern = other.m_NamePattern;
			m_FileDataValidaty = false;
			m_FindHandle = INVALID_HANDLE_VALUE;
		}

		return *this;
	}

	virtual bool			SetDir(const tchar* dir);
	virtual const tstring&	GetDir() const { return m_Dir; }

	virtual bool			CurrentValid() const { return m_FileDataValidaty; }

	virtual bool			MoveNext();

	virtual const WIN32_FIND_DATA& Current();
	
	virtual void			Reset() { Close(); }
	virtual ICloneable*		Clone() const {	return new CDirectoryEnumerator(*this); }

protected:

	void Close();
	bool Open();

	tstring			m_NamePattern;
	tstring			m_Dir;

	bool			m_FileDataValidaty;
	WIN32_FIND_DATA m_FileData;
	HANDLE			m_FindHandle;			
};
