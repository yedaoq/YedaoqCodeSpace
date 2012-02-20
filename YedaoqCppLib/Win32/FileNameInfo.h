/* ___________________________________________________________________________
@ 
@ file - FileName.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-29
@ info -
@     文件名解析与合成
/* ___________________________________________________________________________*/

#pragma once
#include <tchar.h>

class CFileNameInfo
{
public:
	CFileNameInfo(LPCTSTR path)
		: Drive_(_MAX_DRIVE, '\0')
		, Dir_(_MAX_DIR, '\0')
		, Title_(_MAX_FNAME, '\0')
		, Ext_(_MAX_EXT, '\0')
	{
		if(path)
		{
			/* 
			*  0 : success
			*  EINVAL : invalid parameter
			*  ERANGE : one or more buffer is too short to hold the result, all buffer will be clear.
			*/
			LastErr_ = _tsplitpath_s(path, Drive_.data(), _MAX_DRIVE, Dir_.data(), _MAX_DIR, Title_.data(), _MAX_FNAME, Ext_.data(), _MAX_EXT);
			if(LastErr_)
			{
				throw LastErr_;
			}
		}
	}

	CFileNameInfo(LPCTSTR drive, LPCTSTR dir, LPCTSTR title, LPCTSTR ext)
	{
		SetDrive(drive);
		SetDir(dir);
		SetTitle(title);
		SetExt(ext);
	}

	tstring MakePath() const
	{
		tstring path(_MAX_PATH, '\0');
		LastErr_ = _tmakepath_s(path.data(), _MAX_PATH, Drive_.c_str(), Dir_.c_str(), Title_.c_str(), Ext_.c_str());
		return path;
	}

	void SetDrive(LPCTSTR drive)
	{
		if(drive)
		{
			Drive_ = drive;
		}
		else
		{
			Drive_.clear();
		}
	}

	void SetDir(LPCTSTR dir)
	{
		if (dir)
		{
			Dir_ = dir;
		}
		else
		{
			Dir_.clear();
		}
	}
	void SetTitle(LPCTSTR title)
	{
		if (title)
		{
			Title_ = title;
		}
		else
		{
			Title_.clear();
		}
	}

	void SetExt(LPCTSTR ext)
	{
		if (ext)
		{
			Ext_ = ext;
		}
		else
		{
			Ext_.clear();
		}
	}

	errno_t LastError() const { return LastErr_; }

	
protected:
	tstring Drive_;
	tstring Dir_;
	tstring Title_;
	tstring Ext_;
	errno_t	LastErr_;
};