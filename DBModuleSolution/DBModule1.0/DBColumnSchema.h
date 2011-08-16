#pragma once

#include "mytype.h"
#include <vector>

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
enum EnumCppDataType
{
	CPPINT32,
	CPPUINT32,
	CPPDOUBLE,
	CPPFLOAT,
	CPPSTRING,
	CPPBINARY,
	CPPBOOL,
};

interface DBColumnSchema
{
	typedef unsigned int flag_t;
	typedef unsigned int index_t;

	enum EnumFlag
	{
		None		= 0x0001,
		PrimaryKey	= 0x0002,
		Nullable	= 0x0004,	
		Unique		= 0x0008,
	};

	tstring DBName;
	tstring DBType;
	
	tstring Name;
	tstring Type;
	
	index_t	Index;
	flag_t  Flag;

	bool IsPrimaryKey() { return Flag & PrimaryKey; }
	bool IsNullable()	{ return Flag & Nullable; }
	bool IsUnique()		{ return Flag & Unique; }
	void SetPrimaryKey(bool val){  }

	static DBColumnSchema Invalid;
};

typedef std::vector<DBColumnSchema> DBColumnSchemaCollection;

}