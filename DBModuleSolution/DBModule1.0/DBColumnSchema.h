#pragma once

#include "mytype.h"
#include <vector>

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface	IDBDataType;
	enum		EnumCppDataType;

	struct DBColumnSchema
	{
		typedef unsigned int flag_t;
		typedef unsigned int index_t;

		enum EnumFlag
		{
			None		= 0x0000,
			BuildIn		= 0x0001,	// indicate a column in buildin schema
			DBExist		= 0x0002,	// indicate a column exist in database
			KeyColumn	= 0x0004,	// indicate a column was a key field in object comparison

			DBPrimaryKey= 0x0010,	// indicate a column was a PK in database
			DBNullable	= 0x0020,	// indicate a column is nullable in database
		};

		index_t			Index;		// index of column in CDBRecordBase object
		tstring			Name;		// name of col in App
		EnumCppDataType Type;		// type of col in App(C++ type)

		index_t			DBIndex;	// index of column in database table schema
		tstring			DBName;		// name of column in database table schema
		IDBDataType*	DBType;		// type of column in database table schema

		flag_t			UniqueMask;	// every bit of value '1' of this member indicate that this column is unique in group of all other columns that with value '1' at the same bit
		flag_t			Flag;

		bool IsBuildin() const		{ return Flag & BuildIn; }
		bool IsDBExist() const		{ return Flag & DBExist; }
		bool IsKeyColumn() const	{ return Flag & KeyColumn; }

		bool IsDBNullable() const	{ return Flag & DBNullable; }
		bool IsDBPrimaryKey() const { return Flag & DBPrimaryKey; }

		void SetFlag(flag_t flag, bool bAppend)
		{
			if(bAppend)
			{
				Flag |= flag;
			}
			else
			{
				Flag ^= flag;
			}
		}
	};

	typedef std::vector<DBColumnSchema> DBColumnSchemaVct;

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