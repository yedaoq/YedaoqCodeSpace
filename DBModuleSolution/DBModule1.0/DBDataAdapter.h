#pragma once

#ifndef interface
#define interface struct
#endif

#include <memory>

namespace NSDBModule
{
	interface IDBCommandBuilder;
	interface IDBCommand;
	
	class CDBModule;
	class DBRecordSet;
	class DBRecordEnumPtr;
	class CDBRecordBase;
	class tstring;
	class CDBRecordComparison;

	typedef std::auto_ptr<IEnumerator<tstring>> DBTableEnumPtr;
	typedef std::auto_ptr<IEnumerator<DBColumnSchema>> DBColumnEnumPtr;

interface IDBDataAdapter
{
public:
	virtual ~IDBDataAdapter() = 0 {}

	virtual	int				FillSchema(const CDBModule& module) = 0;
	virtual DBTableEnumPtr	EnumTable() = 0;
	virtual DBColumnEnumPtr EnumColumn(const tstring& tblName) = 0;

	virtual int				Fill(DBRecordSet& recs) = 0;
	virtual DBRecordEnumPtr Select(const CDBRecordBase& condition, const CDBRecordComparison& comparison) = 0;
	virtual DBRecordEnumPtr Select(const IDBCommand& cmd) = 0;
	
	virtual int				Execute(const IDBCommand& cmd) = 0;
};

}