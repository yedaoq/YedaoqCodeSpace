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

struct EnumTWValueType
{
	enum _Type :  TW_UINT16
	{
		INT8	= TWTY_INT8,
		INT16	= TWTY_INT16, 
		INT32	= TWTY_INT32,
		UINT8	= TWTY_UINT8, 
		UINT16	= TWTY_UINT16, 
		UINT32	= TWTY_UINT32, 
		BOOL	= TWTY_BOOL, 
		FIX32	= TWTY_FIX32,
		FRAME	= TWTY_FRAME,
		STR32	= TWTY_STR32, 
		STR64	= TWTY_STR64,
		STR128	= TWTY_STR128, 
		STR255	= TWTY_STR255,
	}
};

// 定义性能协商时，使用的数据容器类型
struct EnumCapDataContainer
{
	enum _Type : TW_UINT16
	{
		ARRAY		= TWON_ARRAY,
		ENUM		= TWON_ENUMERATION,
		ONEVALUE	= TWON_ONEVALUE,
		RANGE		= TWON_RANGE,
	};
};

struct ICapDataContainer
{
	EnumCapDataContainer 
};

HGLOBAL twmake_global_onevalue(EnumTWValueType type, TW_UINT32 val)
{
	CRAII_GlobalMemoryHandle hGlobal(sizeof(TW_ONEVALUE));
	TW_ONEVALUE* pOneValue = hGlobal.Lock<TW_ONEVALUE>();
	pOneValue->ItemType = type;
	pOneValue->Item = val;
	hGlobal.Unlock();
	return hGlobal.Detach();
}

HGLOBAL twmake_global_array(EnumTWValueType type, TW_UINT32 val)
{
	CRAII_GlobalMemoryHandle hGlobal(sizeof(TW_ONEVALUE));
	TW_ONEVALUE* pOneValue = hGlobal.Lock<TW_ONEVALUE>();
	pOneValue->ItemType = type;
	pOneValue->Item = val;
	hGlobal.Unlock();
	return hGlobal.Detach();

	tchar* pt = "sdf \
		sdfa \
		sdfa, \
		sdfa, \
		";
}