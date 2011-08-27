#pragma once
#include "mytype.h"
#include "DBTableSchema.h"
//#include <winnt.h>

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	enum EnumDBDataTypeClass { DBValue, DBString, DBBlob };

	interface IDBDataType : ICloneable
	{
		virtual EnumDBDataTypeClass Class() const = 0;									// 
		virtual tstring				ToString() const = 0;								// get the datatype str
		virtual bool				IsNull(const tchar* val) const = 0;						// whether a val of this type is a db null 

		virtual tstring				Wrapper(const tchar* val) const = 0;						// wrapper a val of this type for db store
		virtual tstring				UnWrapper(const tchar* val) const = 0;					// unwrapper a db val for memory store

		virtual EnumCppDataType		PreferredCppDataType() const = 0;					// the the preferred cppdatatype for store value of this type in memory
		virtual bool				CompatibleWith(EnumCppDataType type) const = 0;		// whether this type can contain all value of another type
		virtual bool				CompatibleWith(const IDBDataType& type) const = 0;	// whether this type can contain all value of a memory type
	};

	interface IDBDataTypeProvider
	{
		virtual IDBDataType*		ParseDBTypeStr(const tstring& type) = 0;
		virtual IDBDataType*		GetPreferredDBType(EnumCppDataType type) = 0;

		virtual bool				DBTypeEnumerable() = 0;
		virtual IEnumerator<IDBDataType*> GetEnumerator() = 0;
	};

	
}

