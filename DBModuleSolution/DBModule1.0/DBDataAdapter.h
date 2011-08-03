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

	typedef std::auto_ptr<IDBCommandBuilder> DBCommandBuilderPtr;

interface IDBDataAdapter
{
public:
	virtual ~IDBDataAdapter() = 0 {}

	virtual	int				FillSchema(const CDBModule& module) = 0;

	virtual int				Fill(DBRecordSet& recs) = 0;
	virtual DBRecordEnumPtr Select(const CDBRecordBase& condition, const CDBRecordComparison& comparison) = 0;
	virtual DBRecordEnumPtr Select(const IDBCommand& cmd) = 0;
	
	virtual int				Execute(const IDBCommand& cmd) = 0;

	virtual DBCommandBuilderPtr CreateCommandBuilder() = 0;

	/*virtual int Insert(const CDBRecordBase& rec) = 0;
	virtual int Update(const CDBRecordBase& rec) = 0;
	virtual int Delete(const CDBRecordBase& rec) = 0;*/
};

}