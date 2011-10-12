#pragma once

#include <set>
#include <memory>
#include "Enumerator.h"
#include "DBRecord.h"
#include "Comaprison.h"
#include "mytype.h"
#include ".\Schema\DBTableSchema.h"
#include "DBRecordComparison.h"

namespace NSDBModule
{
	class IDBRecord;
	class CDBTableSchema;
	class CDBRecordComparison;
	class CDBRecordLessThan;
	class CDBModule;
	class CDBSchemaLoader;

	interface IDBCommandBuilder;

	typedef std::set<CDBRecordBase, CDBRecordLessThan>			DBRecordSet;
	typedef DBRecordSet::const_iterator							DBRecordIterator;
	typedef std::tr1::shared_ptr<IEnumerator<IDBRecord>>		DBRecordEnumPtr;
	typedef std::tr1::shared_ptr<IDBCommandBuilder>				DBCommandBuilderPtr;
	typedef CIteratorEnumerator<DBRecordSet::iterator>			DBRecordEnumerator;

	class CDBTable
	{
		friend class CDBSchemaLoader;
	public:
		CDBTable(CDBModule* module, const CDBTableSchema& schema);
		CDBTable(CDBModule* module);
		CDBTable(const CDBTable& other);

		int Initialize();

		const tstring&					GetName() const			{ return Schema_.Name; }
		const tstring&					GetDBName() const		{ return Schema_.DBName; }
		const CDBRecordComparison&		GetComparison() const	{ return Comparison_; }
		const CDBTableSchema&			GetSchema()	const		{ return Schema_; }
		CDBTableSchema&					GetSchema()				{ return Schema_; }

		CDBModule*						GetDBModule() const		{ return DBModule_; }

		//void SetComparison(const CDBRecordComparison& cmp)		{ Comparison_ = cmp; }

		IEnumerator<DBColumnSchema>*	EnumColumn() const;
		DBRecordEnumerator				EnumRecord() /*const*/;

		int								LoadData();
		int								ClearData();

		bool							DataLoaded() const { return FlagLoaded_; }
		bool							DataDirty() const { return FlagDirty_; }
		bool							Invalidate() { FlagDirty_ = true; }

		int								Find(IDBRecord& rec) /*const*/;
		int								Find(IDBRecord& rec, const CDBRecordComparison& cmp) /*const*/;
		DBRecordEnumerator				FindAll(const IDBRecord& rec, const CDBRecordComparison& cmp) /*const*/;
	
		int								Update(const IDBRecord& cur, const IDBRecord& ori);
		int								Insert(const IDBRecord& rec);
		int								Delete(const IDBRecord& rec);

	protected:
		CDBTable& operator=(const CDBTable&);

		CDBModule*						DBModule_;
		CDBTableSchema					Schema_;
		CDBRecordComparison				Comparison_;
		CDBRecordLessThan				LessThan_;

		DBRecordSet						Records_;
		DBCommandBuilderPtr				CommandBuilder_;
		bool							FlagLoaded_;
		bool							FlagDirty_;
	};

}
