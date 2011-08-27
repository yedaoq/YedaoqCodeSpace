#pragma once

#ifndef interface
#define interface struct
#endif

#include <memory>
#include "mytype.h"

namespace NSDBModule
{
	interface IDBCommandBuilder;
	interface IDBCommand;

	class CDBRecordBase;
	class CDBRecordComparison;

interface IDBDataAdapter
{
public:
	typedef std::tr1::shared_ptr<IEnumerator<tstring>>			DBTableEnumPtr;
	typedef std::tr1::shared_ptr<IEnumerator<DBColumnSchema>>	DBColumnEnumPtr;
	typedef std::tr1::shared_ptr<IEnumerator<CDBRecordBase>>	DBRecordEnumPtr;

public:
	virtual ~IDBDataAdapter() = 0 {}

	//virtual	int				FillSchema(const CDBModule& module) = 0;
	virtual DBTableEnumPtr	EnumTable() = 0;
	virtual DBColumnEnumPtr EnumColumn(const tstring& tblName) = 0;

	//virtual int				Fill(DBRecordSet& recs) = 0;
	virtual DBRecordEnumPtr Select(const CDBRecordBase& condition, const CDBRecordComparison& comparison) = 0;
	virtual DBRecordEnumPtr Select(const IDBCommand& cmd) = 0;
	
	virtual int				Execute(const IDBCommand& cmd) = 0;
};

}