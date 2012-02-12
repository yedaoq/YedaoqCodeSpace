/* ___________________________________________________________________________
@ 
@ file - XferModeInfo.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-19
@ info -
@     提供图像传输模式的相关信息
/* ___________________________________________________________________________*/
#pragma once

#include "Twain.h"
#include "..\CPP\microtype.h"

struct XferModeHeaderInfo
{
	byte bSize;
	byte bMode;

protected:
	XferModeHeaderInfo(byte size, byte mode)
		: bSize(size), bMode(mode)
	{};
};

struct XferModeMemoryInfo : public XferModeHeaderInfo
{
	HDC		hdc;
	HBITMAP	hBmp;

public:
	XferModeMemoryInfo()
		: XferModeHeaderInfo(sizeof(XferModeMemoryInfo), TWSX_MEMORY), hdc(0), hBmp(0)
	{}
};

struct XferModeNativeInfo : public XferModeHeaderInfo
{
	HDC		hdc;
	HBITMAP	hBmp;

public:
	XferModeNativeInfo()
		: XferModeHeaderInfo(sizeof(XferModeNativeInfo), TWSX_NATIVE), hdc(0), hBmp(0)
	{}
};

struct XferModeFileInfo : public XferModeHeaderInfo
{
	LPCTSTR	lpFile;

public:
	XferModeFileInfo()
		: XferModeHeaderInfo(sizeof(XferModeFileInfo), TWSX_FILE), lpFile(0)
	{}
};

