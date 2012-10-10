#include "StdAfx.h"
#include "FileEnumerator.h"
#include <stdexcept>
#include "shlwapi.h"

#pragma comment(lib, "shlwapi.lib")

bool CFileEnumerator::Open()
{
	TCHAR buf[MAX_PATH];
	PathCombine(buf, m_Dir.c_str(), m_NamePattern.c_str());
	m_FindHandle = FindFirstFile(buf, &m_FileData);
	return m_FindHandle != INVALID_HANDLE_VALUE;
}

void CFileEnumerator::Close()
{
	if(m_FindHandle != INVALID_HANDLE_VALUE)
	{
		FindClose(m_FindHandle);
		m_FindHandle = INVALID_HANDLE_VALUE;
	}
}

bool CFileEnumerator::MoveNext()
{	
	if(m_AtEnd) return false;

	if(m_SubEnumerator != 0)
	{
		if(m_SubEnumerator->MoveNext())
			return true;
		else
			CloseSubEnumerator();
	}

	while(true)
	{
		if(m_FindHandle == INVALID_HANDLE_VALUE)
		{
			TCHAR buf[MAX_PATH];
			PathCombine(buf, m_Dir.c_str(), m_NamePattern.c_str());
			m_FindHandle = FindFirstFile(buf, &m_FileData);
			if(m_FindHandle == INVALID_HANDLE_VALUE)
			{
				m_AtEnd = true;
				break;
			}
		}
		else if(!FindNextFile(m_FindHandle, &m_FileData))
		{
			_ASSERT(ERROR_NO_MORE_FILES == GetLastError());
			Close();
			m_AtEnd = true;
			break;
		}

		if(m_FileData.cFileName[0] == '.' && (m_FileData.cFileName[1] == 0 || (m_FileData.cFileName[1] == '.' && m_FileData.cFileName[2] == 0)))
			continue;

		if(m_FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(m_EnumFlag & RECUSIVE)
			{
				tchar buf[MAX_PATH];
				PathCombine(buf, m_Dir.c_str(), m_FileData.cFileName);
				m_SubEnumerator = new CFileEnumerator(buf, m_NamePattern.c_str(), m_EnumFlag);
			}

			if(m_EnumFlag & SUBDIR)
				break;

			if(m_SubEnumerator != 0 && m_SubEnumerator->MoveNext())
				break;
		}
		else
		{
			if(m_EnumFlag & SUBFILE)
				break;
		}
	}

	return m_FindHandle != INVALID_HANDLE_VALUE;
}

const WIN32_FIND_DATA& CFileEnumerator::Current()
{
	_ASSERT(m_FindHandle != INVALID_HANDLE_VALUE);

	if(m_SubEnumerator != 0 && m_SubEnumerator->CurrentValid())	//枚举到子文件夹时，会立即创建子枚举器。所以若要把子文件夹作为一个枚举项，则必须先判断一下子枚举是否已经开始MoveNext
		return m_SubEnumerator->Current();
	else
		return m_FileData;
}

void CFileEnumerator::Reset(const tchar* dir, const tchar* pattern, short enum_flag)
{
	Reset();

	if(0 == dir || 0 == dir[0])
		m_Dir.clear();
	else
		m_Dir = dir;

	if(0 == pattern || 0 == pattern[0])
		m_NamePattern = TEXT("*");
	else
		m_NamePattern = pattern;

	m_EnumFlag = enum_flag;
}

void CFileEnumerator::Reset()
{
	m_AtEnd = false;
	Close();
	CloseSubEnumerator();
}

CFileEnumerator::CFileEnumerator( const tchar* dir, const tchar* pattern /*= 0*/, short enum_flag /*= SUBALL*/ )
{
	if(dir && dir[0])
		m_Dir = dir;

	if(pattern && pattern[0])
		m_NamePattern = pattern;
	else
		m_NamePattern = TEXT("*");

	m_EnumFlag = enum_flag;
}

const tstring& CFileEnumerator::CurrentDir() const
{
	return (m_SubEnumerator != 0 && m_SubEnumerator->CurrentValid()) ? m_SubEnumerator->CurrentDir() : m_Dir;
}
