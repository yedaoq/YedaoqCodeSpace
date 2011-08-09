#pragma once

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	class CDBTableSchema;
	class CDBRecordBase;
	class CDBRecordComparison;
	interface IDBCommand;

	interface IDBCommandBuilder
	{
		virtual ~IDBCommandBuilder() = 0 {}
		virtual int Initialize(CDBTableSchema const*) = 0;

		virtual int GetCmdSelect(const CDBRecordBase&, const CDBRecordComparison&, IDBCommand**) = 0;
		virtual int GetCmdSelect(IDBCommand**) = 0;

		virtual int GetCmdInsert(const CDBRecordBase&, IDBCommand**) = 0;
		virtual int GetCmdDelete(const CDBRecordBase&, IDBCommand**) = 0;
		virtual int GetCmdUpdate(const CDBRecordBase&, const CDBRecordBase&, IDBCommand**) = 0;

		virtual int GetCmdCreate() = 0;

		virtual int ReleaseCmd(IDBCommand *, bool);
	};

	class CDBCommandBuilderBase : public IDBCommandBuilder
	{
	public:
		CDBCommandBuilderBase(CDBTableSchema const* schema)
			: TableSchema_(0)
		{}

		virtual int Initialize(CDBTableSchema const*) = 0;

		virtual int GetCmdSelect(const CDBRecordBase&, const CDBRecordComparison&, IDBCommand**) = 0;
		virtual int GetCmdSelect(IDBCommand**) = 0;

		virtual int GetCmdInsert(const CDBRecordBase&, IDBCommand**) = 0;
		virtual int GetCmdDelete(const CDBRecordBase&, IDBCommand**) = 0;
		virtual int GetCmdUpdate(const CDBRecordBase&, const CDBRecordBase&, IDBCommand**) = 0;

		virtual int GetCmdCreate() = 0;

		virtual int ReleaseCmd(IDBCommand *, bool);

	protected:
		virtual int GenerateConditionStr(const CDBRecordBase&, const CDBRecordComparison&);

	protected:
		CDBTableSchema*		TableSchema_;
	};

}