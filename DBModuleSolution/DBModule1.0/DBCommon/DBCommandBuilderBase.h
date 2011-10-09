#pragma once

#include "DBCommandBuilder.h"
//#include "DBRecord.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	struct IDBRecord;

	class CDBCommandBuilderBase : public IDBCommandBuilder
	{
		#define PREGENERATESQLFORFAST
	public:
		CDBCommandBuilderBase()
			: TableSchema_(0)
		{}

		virtual int Initialize(CDBTableSchema const*);

		virtual int GetCmdSelect(const IDBRecord& rec, const CDBRecordComparison& cmp, IDBCommand** cmd);
		virtual int GetCmdSelect(IDBCommand** cmd);

		virtual int GetCmdInsert(const IDBRecord& rec, IDBCommand**);
		virtual int GetCmdDelete(const IDBRecord& rec, const CDBRecordComparison& cmp, IDBCommand** cmd);
		virtual int GetCmdUpdate(const IDBRecord& ori, const IDBRecord& cur, const CDBRecordComparison& cmp, IDBCommand** cmd);

		virtual int GetCmdCreate(IDBCommand**);

		virtual int ReleaseCmd(IDBCommand *, bool);

		virtual int WrapperValue(const tchar* val, IDBDataType const* type, tstring& buffer);
		virtual int WrapperIdentifier(const tchar* val, tstring& buffer);

	protected:
		virtual int GenerateDBCommand(IDBCommand** cmd, EnumDBCommandType type, const tstring& text);

		virtual int GenerateSqlSelect(const IDBRecord& rec, const CDBRecordComparison& cmp, tstring& buffer);
		virtual int GenerateSqlSelect(tstring& buffer);

		virtual int GenerateSqlInsert(const IDBRecord& rec, tstring& buffer);
		virtual int GenerateSqlDelete(const IDBRecord& rec, const CDBRecordComparison& cmp, tstring& buffer);
		virtual int GenerateSqlUpdate(const IDBRecord& ori, const IDBRecord& cur, const CDBRecordComparison& cmp, tstring& buffer);

		virtual int GenerateSqlCreate(tstring& buffer);

		inline void GenerateConditionStr(const IDBRecord& rec, const CDBRecordComparison& cmp, tstring& buffer);
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