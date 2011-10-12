#pragma once
#include "mytype.h"
#include "Enumerator.h"
#include "..\Module\CPPDataType.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	enum EnumDBDataTypeCategory { Unknow, DBNumeric, DBString, DBBlob };

	interface IDBDataType
	{	
		virtual ~IDBDataType() = 0 {};

		virtual EnumDBDataTypeCategory Category() const = 0;								// 
		virtual const tstring&		ToString() const = 0;	// get the datatype str
		virtual bool				Equal(const IDBDataType* other) const = 0;
		
		virtual bool				IsNull(const tchar* val) const = 0;					// whether a val of this type is a db null 

		virtual tstring				Wrapper(const tchar* val) const = 0;				// wrapper a val of this type for db store
		virtual bool				Wrapper(const tchar* val, tstring& buf) const = 0;
		virtual tstring				UnWrapper(const tchar* val) const = 0;				// unwrapper a db val for memory store

		virtual EnumCppDataType		PreferredCppDataType() const = 0;					// the the preferred cppdatatype for store value of this type in memory
		virtual bool				CompatibleWith(EnumCppDataType type) const = 0;		// whether this type can contain all value of another type
		virtual bool				CompatibleWith(const IDBDataType& type) const = 0;	// whether this type can contain all value of a memory type
	};

	interface IDBDataTypeParser
	{
		~IDBDataTypeParser() {}
		virtual IDBDataType* Parse(const tstring& type) const = 0;
	};

	interface IDBDataTypeProvider
	{
		typedef IEnumerator<IDBDataType*>* DBDataTypeEnumPtr;

		virtual ~IDBDataTypeProvider() {};

		virtual IDBDataType*		ParseDBTypeStr(const tstring& type) = 0;
		virtual IDBDataType*		GetPreferredDBType(EnumCppDataType type) = 0;

		virtual bool				DBTypeEnumerable() = 0;
		virtual DBDataTypeEnumPtr	GetEnumerator() = 0;
	};
}

