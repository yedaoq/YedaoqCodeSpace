#pragma once

#include "mytype.h"
#include "DBCommand.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	class CDBTableSchema;
	class CDBRecordBase;
	class CDBRecordComparison;
	interface IDBCommand;
	interface IDBDataType;

	interface IDBCommandBuilder
	{
		virtual ~IDBCommandBuilder() = 0 {}
		virtual int Initialize(CDBTableSchema const*) = 0;

		virtual int GetCmdSelect(const CDBRecordBase&, const CDBRecordComparison&, IDBCommand**) = 0;
		virtual int GetCmdSelect(IDBCommand**) = 0;

		virtual int GetCmdInsert(const CDBRecordBase&, IDBCommand**) = 0;
		virtual int GetCmdDelete(const CDBRecordBase&, const CDBRecordComparison& cmp,  IDBCommand**) = 0;
		virtual int GetCmdUpdate(const CDBRecordBase&, const CDBRecordBase&, const CDBRecordComparison& cmp, IDBCommand**) = 0;

		virtual int GetCmdCreate() = 0;

		virtual int ReleaseCmd(IDBCommand *, bool);
	};

	class CDBCommandBuilderBase : public IDBCommandBuilder
	{
		#define PREGENERATESQLFORFAST
	public:
		CDBCommandBuilderBase()
			: TableSchema_(0)
		{}

		virtual int Initialize(CDBTableSchema const*);

		virtual int GetCmdSelect(const CDBRecordBase& rec, const CDBRecordComparison& cmp, IDBCommand** cmd);
		virtual int GetCmdSelect(IDBCommand** cmd);

		virtual int GetCmdInsert(const CDBRecordBase& rec, IDBCommand**);
		virtual int GetCmdDelete(const CDBRecordBase& rec, const CDBRecordComparison& cmp, IDBCommand** cmd);
		virtual int GetCmdUpdate(const CDBRecordBase& ori, const CDBRecordBase& cur, const CDBRecordComparison& cmp, IDBCommand** cmd);

		virtual int GetCmdCreate(IDBCommand**) = 0;

		virtual int ReleaseCmd(IDBCommand *, bool);

		virtual int WrapperValue(LPCTSTR val, IDBDataType const* type, tstring& buffer);
		virtual int WrapperIdentifier(LPCTSTR val, tstring& buffer);

	protected:
		virtual int GenerateDBCommand(IDBCommand** cmd, EnumDBCommandType type, const tstring& text);

		virtual int GenerateSqlSelect(const CDBRecordBase& rec, const CDBRecordComparison& cmp, tstring& buffer);
		virtual int GenerateSqlSelect(tstring& buffer);

		virtual int GenerateSqlInsert(const CDBRecordBase& rec, tstring& buffer);
		virtual int GenerateSqlDelete(const CDBRecordBase& rec, const CDBRecordComparison& cmp, tstring& buffer);
		virtual int GenerateSqlUpdate(const CDBRecordBase& ori, const CDBRecordBase& cur, const CDBRecordComparison& cmp, tstring& buffer);

		virtual int GenerateSqlCreate(tstring& buffer);

		inline void GenerateConditionStr(const CDBRecordBase& rec, const CDBRecordComparison& cmp, tstring& buffer);
		inline void GenerateFieldAssignmentStr(const DBColumnSchema& col,const tstring& val, tstring& buffer);
		inline void GenerateFieldConditionStr(const DBColumnSchema& col,const tstring& val, tstring& buffer);

	protected:
		const CDBTableSchema*		TableSchema_;

#ifdef PREGENERATESQLFORFAST
		tstring				SqlSelectHead;
		tstring				SqlUpdateHead;
		tstring				SqlInsertHead;
		tstring				SqlDeleteHead;
		tstring				SqlCreateHead;
#endif
	};

}