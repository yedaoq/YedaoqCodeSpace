#include "StdAfx.h"
#include "FileEnumerator.h"

bool CFileEnumerator::SetDir(LPCTSTR dir)
{
	CString dirNew = dir;
	if(!PathIsDirectory(dirNew))
	{
		LPTSTR buf = dirNew.GetBuffer();
		PathRemoveFileSpec(buf);
		dirNew.ReleaseBuffer();
	}

	if(dirNew != m_Dir.c_str())
	{
		Close();
		m_Dir = dirNew;

		return true;
	}

	return false;
}

bool CFileEnumerator::Open()
{
	TCHAR buf[MAX_PATH];
	PathCombine(buf, m_Dir.c_str(), m_NamePattern.c_str());
	m_FindHandle = FindFirstFile(buf, &m_FileData);
	return m_FileDataValidaty = (m_FindHandle != INVALID_HANDLE_VALUE);
}

void CFileEnumerator::Close()
{
	if(m_FindHandle != INVALID_HANDLE_VALUE)
	{
		FindClose(m_FindHandle);
		m_FindHandle = INVALID_HANDLE_VALUE;
		m_FileDataValidaty = false;
	}
}

bool CFileEnumerator::MoveNext()
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