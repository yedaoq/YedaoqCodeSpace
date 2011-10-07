#pragma once

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	enum EnumCppDataType
	{
		CppUnknow = 0,
		CppInt32,
		CppUint32,
		CppDouble,
		CppFloat,
		CppString,
		CppBinary,
		CppBool,
	};
	#define CPPDATATYPECOUNT CppBool
}