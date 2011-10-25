#pragma once

#include "mytype.h"
#include <vector>
#include "EnumEntityHeaderMicro.h"
#include "EnumDBColumnSchemaFlag.h"
#include "..\EnumCppDataType.h"
#include "..\..\DBCommon\DBDataTypeUnknow.h"

namespace NSDBModule
{
	interface	IDBDataType;

	struct DBColumnSchema
	{
		static const int InvalidRelyTableID = -1;
		static const int InvalidRelyColumnID = -1;
		static const int InvalidVisiColumnID = -1;

		index_t			Index;		// index of column in CDBRecordBase object
		tstring			Name;		// name of col in App
		EnumCppDataType Type;		// type of col in App(C++ type)

		index_t			DBIndex;	// index of column in database table schema
		tstring			DBName;		// name of column in database table schema
		IDBDataType*	DBType;		// type of column in database table schema

		int				RelyTblID;	// index of table that this column rely on
		int				RelyColID;	// index of column in table identified by RelyTblID that this column rely on
		int				VisiColID;	// index of column in table identified by RelyTblID that contain visual content of the relyed column

		flag_t			IndexMask;	// every bit of value '1' of this member indicate that this column is index in group of all other columns that with value '1' at the same bit
		flag_t			Flag;

		bool IsBuildin() const		{ return static_cast<bool>(Flag & EnumDBColumnSchemaFlag::BuildIn); }
		bool IsDBExist() const		{ return static_cast<bool>(Flag & EnumDBColumnSchemaFlag::DBExist); }
		bool IsKeyColumn() const	{ return static_cast<bool>(Flag & EnumDBColumnSchemaFlag::KeyColumn); }

		bool IsDBUnnull() const		{ return static_cast<bool>(Flag & EnumDBColumnSchemaFlag::DBUnnull); }
		bool IsDBPrimaryKey() const { return static_cast<bool>(Flag & EnumDBColumnSchemaFlag::DBPrimaryKey); }

		void SetFlag(flag_t flag, bool bAppend)
		{
			if(bAppend)
			{
				Flag |= flag;
			}
			else
			{
				Flag &= ~flag;
			}
		}

		void SetBuildInInfo(const tstring& name, EnumCppDataType type, bool bBuildIn, bool bKeyColumn)
		{
			Name = name;
			Type = type;
			SetFlag(EnumDBColumnSchemaFlag::BuildIn, bBuildIn);
			SetFlag(EnumDBColumnSchemaFlag::KeyColumn, bKeyColumn);
		}

		void SetExternInfo(const tstring& dbName, index_t dbIndex, IDBDataType* dbType, bool bExist, bool bPK, bool bUnnull)
		{
			DBName = dbName;
			DBIndex = dbIndex;
			DBType = dbType;
			SetFlag(EnumDBColumnSchemaFlag::DBExist, bExist);
			SetFlag(EnumDBColumnSchemaFlag::DBPrimaryKey, bPK);
			SetFlag(EnumDBColumnSchemaFlag::DBUnnull, bUnnull);
		}

		void Reset()
		{
			ResetBuildinInfo();
			ResetExternInfo();
			Flag = 0;
			IndexMask = 0;
			RelyTblID = -1;
			RelyColID = -1;
			VisiColID = -1;
		}
		void ResetBuildinInfo()
		{
			Index = -1;
			Name.clear();
			Type = EnumCppDataType::CppUnknow;
			SetFlag(EnumDBColumnSchemaFlag::BuildIn, false);
			SetFlag(EnumDBColumnSchemaFlag::KeyColumn, false);
		}
		void ResetExternInfo()
		{
			DBIndex = -1;
			DBName.clear();
			DBType = &CDBDataTypeUnknow::GetInstance();
			SetFlag(DBExist, false);
			SetFlag(DBPrimaryKey, false);
			SetFlag(DBUnnull, false);
		}
	};

	typedef std::vector<DBColumnSchema> DBColumnSchemaVct;
}