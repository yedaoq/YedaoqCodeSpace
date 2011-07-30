#pragma once

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{

	class DBRecordSet;
	class DBRecordEnumPtr;
	class CDBRecordBase;
	class tstring;
	class CDBRecordComparison;

interface IDBDataAdapter
{
public:
	virtual ~IDBDataAdapter() = 0 {}

	virtual int Fill(DBRecordSet& recs) = 0;

	virtual DBRecordEnumPtr Select(const CDBRecordBase& condition, const CDBRecordComparison& comparison) = 0;
	virtual DBRecordEnumPtr Select(const tstring& sql);

	virtual int Insert(const CDBRecordBase& rec) = 0;
	virtual int Update(const CDBRecordBase& rec) = 0;
	virtual int Delete(const CDBRecordBase& rec) = 0;
};

}