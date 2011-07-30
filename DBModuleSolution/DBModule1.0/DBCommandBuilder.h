#pragma once

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	class CDBTableSchema;
	class CDBRecordBase;
	class CDBRecordComparison;

	interface IDBCommandBuilder
	{
		virtual ~IDBCommandBuilder() = 0 {}
		virtual int Initialize() = 0;

		virtual int GetCmdSelect(const CDBRecordBase&, const CDBRecordComparison&) = 0;
		virtual int GetCmdInsert(const CDBRecordBase&) = 0;
		virtual int GetCmdUpdate(const CDBRecordBase&) = 0;
		virtual int GetCmdDelete(const CDBRecordBase&) = 0;
	};

	class CDBCommandBuilderBase : public IDBCommandBuilder
	{
	public:
		CDBCommandBuilderBase(const CDBTableSchema& schema)
			: TableSchema_(schema)
		{}
		
		virtual int Initialize();

		virtual int GetCmdSelect(const CDBRecordBase&, const CDBRecordComparison&);
		virtual int GetCmdInsert(const CDBRecordBase&);
		virtual int GetCmdUpdate(const CDBRecordBase&);
		virtual int GetCmdDelete(const CDBRecordBase&);

	protected:
		const CDBTableSchema& TableSchema_;
	};

}