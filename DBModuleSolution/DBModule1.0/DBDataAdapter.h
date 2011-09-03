#pragma once

#ifndef interface
#define interface struct
#endif

#include <memory>
#include "mytype.h"
#include "Enumerator.h"

namespace NSDBModule
{
	interface IDBCommandBuilder;
	interface IDBCommand;
	interface IDBRecord;

	class CDBRecordBase;	
	class CDBRecordComparison;

	struct DBColumnSchema;

	interface IDBDataAdapter
	{
	public:
		typedef std::tr1::shared_ptr<IEnumerator<tstring>>			DBTableEnumPtr;
		typedef std::tr1::shared_ptr<IEnumerator<DBColumnSchema>>	DBColumnEnumPtr;
		typedef std::tr1::shared_ptr<IEnumerator<IDBRecord>>		DBRecordEnumPtr;

	public:
		virtual ~IDBDataAdapter() = 0 {}

		virtual DBTableEnumPtr		EnumTable() = 0;
		virtual DBColumnEnumPtr		EnumColumn(const tstring& tblName) = 0;			

		virtual DBRecordEnumPtr		Select(const IDBCommand& cmd) = 0;
		virtual int					Execute(const IDBCommand& cmd) = 0;
	};

}