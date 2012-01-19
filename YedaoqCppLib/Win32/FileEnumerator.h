#pragma once

#include "..\CPP\Enumerator.h"
#include <winbase.h>
#include <string>
#include "..\cpp\tstring.h"

class CFileEnumerator : CEnumeratorBase<WIN32_FIND_DATA>
{
public:
	~CFileEnumerator()
	{
		if (INVALID_HANDLE_VALUE != m_FindHandle)
		{
			Close();
		}
	}

	CFileEnumerator()
		: m_Dir(TEXT(""))
		, m_NamePattern(TEXT("*"))
		, m_FileDataValidaty(false)
		, m_FindHandle(INVALID_HANDLE_VALUE)
	{}

	CFileEnumerator(LPCTSTR lpDir, LPCTSTR lpNamePattern = TEXT("*"))
		: m_NamePattern(lpNamePattern)
		, m_FileDataValidaty(false)
		, m_FindHandle(INVALID_HANDLE_VALUE)
	{
		SetDir(lpDir);
	}

	CFileEnumerator(const CFileEnumerator& other)
		: m_Dir(other.m_Dir)
		, m_NamePattern(other.m_NamePattern)
		, m_FileDataValidaty(false)
		, m_FindHandle(INVALID_HANDLE_VALUE)
	{}

	CFileEnumerator& operator=(const CFileEnumerator& other)
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
	virtual ICloneable*		Clone() const {	return new CFileEnumerator(*this); }

protected:

	void Close();
	bool Open();

	tstring			m_NamePattern;
	tstring			m_Dir;

	bool			m_FileDataValidaty;
	WIN32_FIND_DATA m_FileData;
	HANDLE			m_FindHandle;			
};
