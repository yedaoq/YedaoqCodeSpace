/* ___________________________________________________________________________
@ 
@ file - FileEnumerator.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-29
@ info -
@     文件目录枚举器
/* ___________________________________________________________________________*/

#pragma once

#include <winbase.h>
#include "Enumerator.h"
#include <string>
#include "tstring.h"
#include "initalized_num.h"

class CFileEnumerator : CEnumeratorBase<WIN32_FIND_DATA>
{
public:
	enum EnumFlag : short { SUBFILE = 1, SUBDIR = 2, SUBALL = 3, RECUSIVE = 4 };

public:
	~CFileEnumerator()
	{
		if (INVALID_HANDLE_VALUE != m_FindHandle)
		{
			Close();
		}
	}

	CFileEnumerator() {}

	CFileEnumerator(LPCTSTR lpDir, LPCTSTR lpNamePattern = NULL)
	{
		Open(lpDir);
	}

	CFileEnumerator(const CFileEnumerator& other)
		: m_Dir(other.m_Dir)
		, m_NamePattern(other.m_NamePattern)
		, m_EnumFlag(other.m_EnumFlag)
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

	virtual bool			Open(const tchar* dir);
	virtual const tstring&	GetDir() const { return m_Dir; }

	virtual bool			CurrentValid() const { return m_FileDataValidaty; }

	virtual bool			MoveNext();

	virtual const WIN32_FIND_DATA& Current();
	
	virtual void			Reset(const tchar* dir, short enum_flag);
	virtual void			Reset() { Close(); }
	virtual ICloneable*		Clone() const {	return new CFileEnumerator(*this); }

protected:

	void Close();
	bool Open();

	tstring											m_NamePattern;
	tstring											m_Dir;
	initalized_num<short, SUBALL>					m_EnumFlag;
	initalized_num<bool, false>						m_FileDataValidaty;

	WIN32_FIND_DATA									m_FileData;
	initalized_num<HANDLE, INVALID_HANDLE_VALUE>	m_FindHandle;	

	initalized_num<CFileEnumerator*, 0>				m_SubEnumerator;
};
