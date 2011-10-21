#pragma once

#include "Enumerator.h"
#include <stdexcept>
#include <winbase.h>
#include "mytype.h"

class CFileEnumerator : public CEnumeratorBase<WIN32_FIND_DATA>
{
public:
	~CFileEnumerator()
	{
		Close();
	}

	CFileEnumerator()
		: m_Dir(TEXT("")), m_NamePattern(TEXT("*")), m_FileDataValidaty(false), m_FindHandle(INVALID_HANDLE_VALUE)
	{}

	CFileEnumerator(LPCTSTR lpDir, LPCTSTR lpNamePattern = TEXT("*"))
		: m_NamePattern(lpNamePattern), m_FileDataValidaty(false), m_FindHandle(INVALID_HANDLE_VALUE)
	{
		SetDir(lpDir);
	}

	CFileEnumerator(const CFileEnumerator& other)
		: m_Dir(other.m_Dir), m_NamePattern(other.m_NamePattern), m_FileDataValidaty(false), m_FindHandle(INVALID_HANDLE_VALUE)
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

	virtual bool SetDir(LPCTSTR dir);

	virtual const tstring& GetDir() const { return m_Dir; }

	virtual bool MoveNext();

	virtual bool MoveNext(WIN32_FIND_DATA& obj)
	{
		if(MoveNext())
		{
			obj = Current();
			return true;
		}
		return false;
	}

	virtual const WIN32_FIND_DATA& Current()
	{
		if(!m_FileDataValidaty)
		{
			_ASSERT(false);
			throw std::out_of_range("Ã¶¾ÙÆ÷³¬³ö·¶Î§£¡");
		}
		return m_FileData;
	}

	virtual bool CurrentValid(){ return m_FileDataValidaty; }

	virtual void Reset()
	{
		Close();
	}

	virtual ICloneable* Clone() const
	{
		return new CFileEnumerator(*this);
	}

protected:

	void Close();
	bool Open();

	tstring			m_NamePattern;
	tstring			m_Dir;

	bool			m_FileDataValidaty;
	WIN32_FIND_DATA m_FileData;
	HANDLE			m_FindHandle;			
};
