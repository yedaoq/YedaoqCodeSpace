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

		tstring DBName;
		tstring DBType;

		tstring Name;
		tstring Type;

		index_t	Index;
		flag_t  Flag;

		bool IsPrimaryKey() { return Flag & PrimaryKey; }
		bool IsNullable()	{ return Flag & Nullable; }
		bool IsUnique()		{ return Flag & Unique; }
		
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

	class CDBTableSchema
	{
	public:
		tstring				DBName;
		tstring				Name;
		DBColumnSchemaVct	ColumnSchema;

		const DBColumnSchema& operator[](const tstring& col);
		const DBColumnSchema& operator[](index_t col);
		//ColumnSchemaEnumPtr	  GetColumnSchemaEnumerator();
	};


}
