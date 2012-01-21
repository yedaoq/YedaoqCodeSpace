/* ___________________________________________________________________________
@ 
@ file - TWEnum.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-21
@ info -
@     
/* ___________________________________________________________________________*/
#pragma once

// TWAIN中使用的值类型
struct EnumValueType
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
struct EnumDataContainerType
{
	enum _Type : TW_UINT16
	{
		ARRAY		= TWON_ARRAY,
		ENUM		= TWON_ENUMERATION,
		ONEVALUE	= TWON_ONEVALUE,
		RANGE		= TWON_RANGE,
	};
};