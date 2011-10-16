#pragma once

#include "EnumEntityHeaderMicro.h"

namespace NSDBModule
{
	BEGINENUM(CppDataType)

	ENUMITEM3(CppUnknow,	0,	unknow)
	ENUMITEM1(CppInt32,			int)
	ENUMITEM1(CppUint32,		unsigned int)
	ENUMITEM1(CppDouble,		double)
	ENUMITEM1(CppFloat,			float)
	ENUMITEM1(CppString,		tstring)
	ENUMITEM1(CppBinary,		byte*)
	ENUMITEM1(CppBool,			bool)

	ENDENUM
}