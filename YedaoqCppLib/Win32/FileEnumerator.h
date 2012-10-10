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

class CFileEnumerator : public CCloneable<CFileEnumerator, CEnumeratorBase<WIN32_FIND_DATA>>
{
public:
	enum EnumFlag : short { SUBFILE = 1, SUBDIR = 2, SUBALL = 3, RECUSIVE = 4 };

public:
	~CFileEnumerator() { Close(); }
	CFileEnumerator() {}

	CFileEnumerator(const tchar* dir, const tchar* pattern = 0, short enum_flag = SUBALL);

	CFileEnumerator(const CFileEnumerator& other)
		: m_Dir(other.m_Dir)
		, m_NamePattern(other.m_NamePattern)
		, m_EnumFlag(other.m_EnumFlag)
	{}

	CFileEnumerator& operator=(const CFileEnumerator& other)
	{
		if(this != &other)
		{
			Reset(other.m_Dir.c_str(), other.m_NamePattern.c_str(), other.m_EnumFlag);
		}

		return *this;
	}

	virtual const tstring&	GetDir() const { return m_Dir; }

	virtual bool			CurrentValid() const { return m_FindHandle != INVALID_HANDLE_VALUE; }

	virtual bool			MoveNext();

	virtual const WIN32_FIND_DATA& Current();
	
	void					Reset(const tchar* dir, const tchar* pattern = 0, short enum_flag = SUBALL);
	virtual void			Reset();
	//virtual ICloneable*		Clone() const {	return new CFileEnumerator(*this); }

protected:
	bool Open();
	void Close();
	void CloseSubEnumerator() { if(m_SubEnumerator != 0) { delete m_SubEnumerator; m_SubEnumerator = 0; } }

	tstring											m_NamePattern;
	tstring											m_Dir;
	initalized_num<short, SUBALL>					m_EnumFlag;
	initalized_num<bool, false>						m_AtEnd;

	WIN32_FIND_DATA									m_FileData;
	initalized_num<HANDLE, INVALID_HANDLE_VALUE>	m_FindHandle;	

	initalized_num<CFileEnumerator*, 0>				m_SubEnumerator;
};
