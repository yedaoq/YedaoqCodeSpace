#pragma once
#include "mytype.h"
#include "DBTableSchema.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	enum EnumDBDataTypeClass { DBValue, DBString, DBBlob };

	interface IDBDataType : ICloneable
	{
		virtual EnumDBDataTypeClass Class() = 0;
		virtual tstring				ToString() = 0;
		virtual bool				IsNull(tstring val) = 0;

		virtual EnumCppDataType		PreferredCppDataType() = 0;
		virtual bool				CompatibleWith(EnumCppDataType type) = 0;
		virtual EnumCppDataType		CompatibleWith(const IDBDataType& type) = 0;
	};

	interface IDBataTypeProvider
	{
		virtual IDBDataType*		ParseDBTypeStr(tstring type) = 0;
		virtual IDBDataType*		GetPreferredDBType(EnumCppDataType type) = 0;

		virtual bool				DBTypeEnumerable() = 0;
		virtual IEnumerator<IDBDataType*> GetEnumerator() = 0;
	};
}

