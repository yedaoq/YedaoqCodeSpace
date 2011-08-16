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

	struct DBColumnSchema
	{
		typedef unsigned int flag_t;
		typedef unsigned int index_t;

		enum EnumFlag
		{
			None		= 0x0000,
			PrimaryKey	= 0x0001,
			Nullable	= 0x0002,
			Unique		= 0x0004,

			DBPrimaryKey= 0x0010;
			DBNullable	= 0x0020;
			DBUnique	= 0x0040;

			BuildIn		= 0x0100;
			DBExist		= 0x0200;
		};

		index_t			Index;		// index of column in CDBRecordBase object
		tstring			Name;		// name of col in App
		EnumCppDataType Type;		// type of col in App(C++ type)

		index_t			DBIndex;	// index of column in database table schema
		tstring			DBName;		// name of column in database table schema
		IDBDataType*	DBType;		// type of column in database table schema

		
		flag_t  Flag;

		bool IsPrimaryKey() const	{ return Flag & PrimaryKey; }
		bool IsNullable() const		{ return Flag & Nullable; }
		bool IsUnique()	const		{ return Flag & Unique; }
		bool IsBuildin() const		{ return Flag & BuildIn; }
		bool IsDBExist() const		{ return Flag & DBExist; }
		
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

		bool CompatibleWith(const DBColumnSchema& other);

	protected:
		
		static DBColumnSchema Invalid;
	};

	typedef std::vector<DBColumnSchema>								DBColumnSchemaCollection;
	typedef CIteratorEnumerator<DBColumnSchemaCollection::iterator> DBColumnSchemaEnumerator;

	class CDBTableSchema
	{
	public:
		tstring						DBName;
		tstring						Name;
		DBColumnSchemaCollection	Columns;

	public:
		//CDBTableSchema(){}

		CDBTableSchema(const tstring& dbName, const tstring& name)
			: DBName(dbName), Name(name), Columns(0)
		{}

		DBColumnSchema&			operator[](const tstring& dbCol);
		DBColumnSchema&			operator[](index_t col);

		const DBColumnSchema&	operator[](const tstring& dbCol) const
		{
			return const_cast<CDBTableSchema&>(*this)[col];
		}
		const DBColumnSchema&	operator[](index_t col) const
		{
			return const_cast<CDBTableSchema&>(*this)[col];
		}

		bool					find(const tstring& dbCol, DBColumnSchema** colSchema);
		bool					find(index_t colIdx, DBColumnSchema** colSchema);

		bool					Validate();
		void					Clear();
		bool					Load(IDBDataAdapter* dbAdapter);
	};
}
