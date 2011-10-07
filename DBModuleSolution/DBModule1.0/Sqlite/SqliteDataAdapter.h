#pragma once

#include "DBDataAdapter.h"
//#include "CppSQLite3U.h"
#include "I_DBControl.h"
#include "Enumerator.h"
#include "mytype.h"
#include "DBRecord.h"

namespace NSDBModule
{
	struct DBColumnSchema;

	class CSqliteDataAdapter : public IDBDataAdapter
	{
	public:
		CSqliteDataAdapter(I_CppSQLite3DB* db)
			: SqliteDB(db)
		{}

		virtual ~CSqliteDataAdapter() {}

		virtual DBTableEnumPtr		EnumTable();
		virtual DBColumnEnumPtr		EnumColumn(const tstring& tblName);

		virtual DBRecordEnumPtr		Select(const IDBCommand& cmd);
		virtual int					Execute(const IDBCommand& cmd);

	protected:
		I_CppSQLite3DB* SqliteDB;
	};

	class CSqliteTableEnumerator : public IEnumerator<tstring>
	{
	public:
		CSqliteTableEnumerator(const CAutoDBObjPtr<I_CppSQLite3Query>& query)
			: QueryPtr(query), BeforeFirst(true)
		{}

		virtual bool MoveNext();
		virtual const tstring& Current();
		virtual bool MoveNext(tstring& tbl);
		virtual void Reset() { }

		virtual ICloneable* Clone() const { return new CSqliteTableEnumerator(QueryPtr); }

	protected:
		CAutoDBObjPtr<I_CppSQLite3Query> QueryPtr;
		bool							 BeforeFirst;
	};

	class CSqliteColumnEnumerator : public IEnumerator<DBColumnSchema>
	{
	public:
		CSqliteColumnEnumerator(const CAutoDBObjPtr<I_CppSQLite3Query>& query)
			: QueryPtr(query), IdxCol(-1)
		{}

		virtual bool MoveNext();
		virtual const DBColumnSchema& Current();
		virtual bool MoveNext(DBColumnSchema& tbl);
		virtual void Reset() { IdxCol = -1; }

		virtual ICloneable* Clone() const { return new CSqliteColumnEnumerator(QueryPtr); }

	protected:
		CAutoDBObjPtr<I_CppSQLite3Query>	QueryPtr;
		int									IdxCol;
		
		static tstring						DataTypeStrs[];
	};

	class CSqliteRecordEnumerator : public IEnumerator<IDBRecord>
	{
	public:
		class CSqliteRecord : public IDBRecord
		{
		public:
			CSqliteRecord(I_CppSQLite3Query* query)
				: QueryPtr(query)
			{}

			virtual const tstring&	GetField(unsigned int idx) const { return QueryPtr->getStringField(idx); }
			virtual int				SetField(unsigned int idx, const tstring& val) { throw std::exception(); }
			virtual index_t			GetFieldCount() const { return QueryPtr->numFields(); };

		protected:
			I_CppSQLite3Query*		QueryPtr;
		};

	public:
		CSqliteRecordEnumerator(const CAutoDBObjPtr<I_CppSQLite3Query>& query)
			: QueryPtr(query), BeforeFirst(true), Record(QueryPtr)
		{}

		virtual bool MoveNext();
		virtual const IDBRecord& Current();
		virtual bool MoveNext(IDBRecord& tbl);
		virtual void Reset() { }

		virtual ICloneable* Clone() const { return new CSqliteRecordEnumerator(QueryPtr); }

	protected:
		CAutoDBObjPtr<I_CppSQLite3Query> QueryPtr;
		bool							 BeforeFirst;
		CSqliteRecord					 Record;
	};
}