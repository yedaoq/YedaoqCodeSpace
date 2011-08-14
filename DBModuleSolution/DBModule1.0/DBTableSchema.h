#pragma once
#include "mytype.h"
#include <vector>
#include <memory>
#include "Enumerator.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface IDBDataType;

	enum EnumCppDataType
	{
		CppUnknow,
		CppInt32,
		CppUint32,
		CppDouble,
		CppFloat,
		CppString,
		CppBinary,
		CppBool,
	};

#define CPPDATATYPECOUNT CppBool

	struct DBColumnSchema
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

		tstring			Name;
		EnumCppDataType Type;

		tstring			DBName;
		IDBDataType*	DBType;

		index_t	Index;
		flag_t  Flag;

		bool IsPrimaryKey() const	{ return Flag & PrimaryKey; }
		bool IsNullable() const		{ return Flag & Nullable; }
		bool IsUnique()	const		{ return Flag & Unique; }
		
		void SetPrimaryKey(bool val)
		{
			SetFlag(EnumFlag::PrimaryKey, val);
		}

		void SetNullable(bool val)
		{
			SetFlag(EnumFlag::Nullable, val);
		}

		void SetUnique(bool val)
		{
			SetFlag(EnumFlag::Unique, val);
		}

	protected:
		void SetFlag(EnumFlag flag, bool bAppend)
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

		static DBColumnSchema Invalid;
	};

	typedef std::vector<DBColumnSchema> DBColumnSchemaVct;
	typedef CIteratorEnumerator<DBColumnSchemaVct::iterator> DBColumnSchemaEnumerator;

	class CDBTableSchema
	{
	public:
		tstring				DBName;
		tstring				Name;
		DBColumnSchemaVct	ColumnSchema;

		const DBColumnSchema& operator[](const tstring& col) const
		{
			return const_cast<CDBTableSchema&>(*this)[col];
		}
		const DBColumnSchema& operator[](index_t col) const
		{
			return const_cast<CDBTableSchema&>(*this)[col];
		}
		
		DBColumnSchema& operator[](const tstring& col);
		DBColumnSchema& operator[](index_t col);
	};
}
