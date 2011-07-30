#include "DBTable.h"

using namespace NSDBModule;

CDBTable::CDBTable()
	: FlagLoaded_(false)
{

}

ColumnSchemaEnumPtr CDBTable::EnumColumn()
{
	return Schema_.GetColumnSchemaEnumerator();
}

DBRecordEnumPtr	CDBTable::EnumRecord()
{
	return DBRecordEnumPtr(new CIteratorEnumerator<CDBRecordBase>(Records_.begin(), Records_.end()));
}

int	CDBTable::LoadData()
{
	if(FlagLoaded_)
	{
		return 1;
	}

	Records_.clear();
	//fill

	return 1;
}

int	CDBTable::FindData(CDBRecordBase& rec)
{
	RecIterator iter = Records_.find(rec);
	if(iter != Records_.end())
	{
		rec = *iter;
		return 1;
	}

	return 0;
}

int	CDBTable::FindData(CDBRecordBase& rec, const CDBRecordComparison& cmp)
{
	for (RecIterator iter = Records_.begin(); iter != Records_.end(); ++iter)
	{
		if(0 == cmp(rec, *iter))
		{
			rec = *iter;
			return 1;
		}
	}
	return 0;
}

int	CDBTable::Update(const CDBRecordBase& cur, const CDBRecordBase& ori)
{

}

int	CDBTable::Insert(const CDBRecordBase& rec)
{

}

int	CDBTable::Delete(const CDBRecordBase& rec)
{

}