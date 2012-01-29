#include "StdAfx.h"
#include "DirectoryEnumerator.h"
#include <stdexcept>
#include "shlwapi.h"

bool CDirectoryEnumerator::SetDir(const tchar* dir)
{
	if(!dir) { return false; }

	tchar buf[MAX_PATH];
	StrCpyN(buf, dir, MAX_PATH);

	if(!PathIsDirectory(buf))
	{
		PathRemoveFileSpec(buf);
		if(!PathIsDirectory(buf)) 
		{	
			return false; 
		}
	}	

	Close();
	m_Dir = buf;
	return true;
}

bool CDirectoryEnumerator::Open()
{
	TCHAR buf[MAX_PATH];
	PathCombine(buf, m_Dir.c_str(), m_NamePattern.c_str());
	m_FindHandle = FindFirstFile(buf, &m_FileData);
	return m_FileDataValidaty = (m_FindHandle != INVALID_HANDLE_VALUE);
}

void CDirectoryEnumerator::Close()
{
	if(m_FindHandle != INVALID_HANDLE_VALUE)
	{
		FindClose(m_FindHandle);
		m_FindHandle = INVALID_HANDLE_VALUE;
		m_FileDataValidaty = false;
	}
}

bool CDirectoryEnumerator::MoveNext()
{
	if(INVALID_HANDLE_VALUE == m_FindHandle)
	{
		return Open();
	}

	if(m_FileDataValidaty && m_FindHandle)
	{
		m_FileDataValidaty = FindNextFile(m_FindHandle, &m_FileData);
		if(!m_FileDataValidaty)
		{
			_ASSERT(ERROR_NO_MORE_FILES == GetLastError());
		}
	}

	return m_FileDataValidaty;
}

const WIN32_FIND_DATA& CDirectoryEnumerator::Current()
{
	if(!m_FileDataValidaty)
	{
		_ASSERT(false);
		throw std::out_of_range("Ã¶¾ÙÆ÷³¬³ö·¶Î§£¡");
	}
	return m_FileData;
}
