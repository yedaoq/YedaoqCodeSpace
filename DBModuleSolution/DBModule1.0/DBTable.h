#pragma once

#include <set>
#include <memory>
#include "Enumerator.h"
#include "Comaprison.h"
#include "mytype.h"
#include "DBTableSchema.h"
#include "DBRecordComparison.h"
#include "DBCollection.h"

namespace NSDBModule
{

class CDBRecordBase;
class CDBTableSchema;
class CDBRecordComparison;
class CDBRecordLessThan;
class CDBModule;

interface IDBCommandBuilder;

typedef std::auto_ptr<IEnumerator<CDBRecordBase>>				DBRecordEnumPtr;
typedef std::set<CDBRecordBase, IBoolComparison<CDBRecordBase>&>	DBRecordSet;
typedef DBRecordSet::const_iterator								RecIterator;
typedef std::auto_ptr<IDBCommandBuilder>						DBCommandBuilderPtr;
typedef CIteratorEnumerator<DBRecordSet::const_iterator>		DBRecordEnumerator;

class CDBTable
{
public:
	CDBTable(CDBModule* module);
	CDBTable(const CDBTable& other);

	int Initialize();

	const tstring&					GetName()		{ return Schema_.Name; }
	const tstring&					GetDBName()		{ return Schema_.DBName; }
	const CDBRecordComparison&		GetComparison() { return Comparison_; }
	const CDBTableSchema&			GetSchema()		{ return Schema_; }

	CDBTableSchema::ColumnEnumerator EnumColumn();
	DBRecordEnumerator				EnumRecord();

	int								LoadData();
	int								ClearData();

	int								Find(CDBRecordBase& rec);
	int								Find(CDBRecordBase& rec, const CDBRecordComparison& cmp);
	RecEnumPtr						FindAll(const CDBRecordBase& rec, const CDBRecordComparison& cmp);
	
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

};

}
