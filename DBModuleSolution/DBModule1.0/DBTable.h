#pragma once

#include <set>
#include <memory>
#include "Enumerator.h"
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
class IDBCommandBuilder;
class CDBSchema;

typedef std::auto_ptr<IEnumerator<CDBRecordBase>>				DBRecordEnumPtr;
typedef std::auto_ptr<IDBCommandBuilder>						DBCommandBuilderPtr;
typedef std::set<CDBRecordBase, IComparison<CDBRecordBase>&>	DBRecordSet;
typedef DBRecordSet::const_iterator								RecIterator;

class CDBTable
{
public:
	CDBTable();

	const tstring&		GetName()	{ return Schema_.Name; }
	const tstring&		GetDBName()	{ return Schema_.DBName; }

	const CDBRecordComparison& GetComparison() { return Comparison_; }

	ColumnSchemaEnumPtr EnumColumn();
	DBRecordEnumPtr		EnumRecord();

	int					LoadData();
	int					FindData(CDBRecordBase& rec);
	int					FindData(const CDBRecordBase& rec, const CDBRecordComparison& cmp);
	
	int					Update(const CDBRecordBase& cur, const CDBRecordBase& ori);
	int					Insert(const CDBRecordBase& rec);
	int					Delete(const CDBRecordBase& rec);

protected:
	CDBSchema*			DBSchema_;
	CDBTableSchema		Schema_;
	CDBRecordComparison	Comparison_;
	CComparisonToLessThen<CDBRecordBase> LessThan_;

	DBRecordSet			Records_;
	DBCommandBuilderPtr	Command_;

	bool				FlagLoaded_;
};

}
