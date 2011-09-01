#pragma once

#include <set>
#include <memory>
#include "Enumerator.h"
#include "DBRecord.h"
#include "Comaprison.h"
#include "mytype.h"
#include "DBTableSchema.h"
#include "DBRecordComparison.h"

namespace NSDBModule
{
	class CDBRecordBase;
	class CDBTableSchema;
	class CDBRecordComparison;
	class CDBRecordLessThan;
	class CDBModule;
	class CDBSchemaLoader;

	interface IDBCommandBuilder;

	typedef std::set<CDBRecordBase, CDBRecordLessThan&>				DBRecordSet;
	typedef DBRecordSet::const_iterator								DBRecordIterator;
	typedef std::tr1::shared_ptr<IEnumerator<CDBRecordBase>>		DBRecordEnumPtr;
	typedef std::tr1::shared_ptr<IDBCommandBuilder>					DBCommandBuilderPtr;
	typedef CIteratorEnumerator<DBRecordSet::const_iterator>		DBRecordEnumerator;

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

		//void SetComparison(const CDBRecordComparison& cmp)		{ Comparison_ = cmp; }

		CDBTableSchema::ColumnEnumerator EnumColumn() const;
		DBRecordEnumerator				EnumRecord() /*const*/;

		int								LoadData();
		int								ClearData();

		bool							DataLoaded() const { return FlagLoaded_; }
		bool							DataDirty() const { return FlagDirty_; }
		bool							Invalidate() { FlagDirty_ = true; }

		int								Find(CDBRecordBase& rec) /*const*/;
		int								Find(CDBRecordBase& rec, const CDBRecordComparison& cmp) /*const*/;
		DBRecordEnumerator				FindAll(const CDBRecordBase& rec, const CDBRecordComparison& cmp) /*const*/;
	
		int								Update(const CDBRecordBase& cur, const CDBRecordBase& ori);
		int								Insert(const CDBRecordBase& rec);
		int								Delete(const CDBRecordBase& rec);

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
