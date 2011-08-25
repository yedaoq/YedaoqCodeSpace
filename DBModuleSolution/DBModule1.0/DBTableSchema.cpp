#include "DBTableSchema.h"
#include <stdexcept>
#include <algorithm>
#include "DBDataAdapter.h"

using namespace NSDBModule;

bool CDBTableSchema::FindByName(const tstring& col, DBColumnSchema& colSchema)
{
	for (ColumnCollection::iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
	{
		if(col == iter->Name)
		{
			colSchema = *iter;
			return true;
		}
	}
	return false;
}

bool CDBTableSchema::FindByIndex(index_t col, DBColumnSchema& colSchema)
{
	if(col < Columns.size())
	{
		_ASSERT(Columns[col].Index == col);
		colSchema = Columns[col];
		return true;
	}
	return false;
}

bool CDBTableSchema::ModifyColumn(const DBColumnSchema& col)
{
	if(col.Index < Columns.size())
	{
		Columns[col.Index] = col;
		return true;
	}

	return false;
}

bool CDBTableSchema::AppendColumn(DBColumnSchema& col)
{
	col.Index = Columns.size();
	Columns.push_back(col);
}

DBColumnSchema& CDBTableSchema::operator[](const tstring& dbCol)
{
	for (ColumnCollection::iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
	{
		if(dbCol == iter->DBName)
		{
			return *iter;
		}
	}

	throw std::out_of_range("");
}

DBColumnSchema& CDBTableSchema::operator[](index_t col)
{
	if(col < Columns.size())
	{
		_ASSERT(Columns[col].Index == col);
		return Columns[col];
	}

	throw std::out_of_range("");
}

void CDBTableSchema::Clear()
{
	DBName.clear();
	SetFlag(DBExist, false);

	// remove all not buildin column
	Columns.erase
		(
			std::remove_if
			(
				Columns.begin(), Columns.end(), 
				std::not1(std::mem_fun(&DBColumnSchema::IsBuildin))
			),
			Columns.end()
		);

	// reset flag of all column
	for(ColumnCollection::iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
	{
		iter->SetFlag(DBColumnSchema::DBExist | DBColumnSchema::DBPrimaryKey, false);
		iter->SetFlag(DBColumnSchema::DBNullable, true);
		iter->UniqueMask = 0;
	}
}

bool CDBTableSchema::Load(IDBDataAdapter* dbAdapter)
{
	Clear();

	// get database columns
	DBColumnEnumPtr pEnum = dbAdapter->EnumColumn(DBName);
	if(!pEnum.get())
	{
		//assert
		return false;
	}

	// merge columns schema
	pEnum->Reset();
	while(pEnum->MoveNext())
	{
		const DBColumnSchema& dbCol = pEnum->Current();
		DBColumnSchema col;
		if(!FindByName(dbCol.DBName, col))
		{
			col.Name = dbCol.DBName;			
			Columns.push_back(dbCol);
		}

		dbCol.SetFlag(DBColumnSchema::BuildIn, false);
		dbCol.SetFlag(DBColumnSchema::KeyColumn, false);

		col.Type = dbCol.DBType->PreferredCppDataType();
		col.Flag |= DBExist | dbCol.Flag;
		ModifyColumn(col);
	}

	//
}