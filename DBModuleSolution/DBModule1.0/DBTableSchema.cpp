#include "DBTableSchema.h"
#include <stdexcept>

using namespace NSDBModule;

bool CDBTableSchema::FindByDBName(const tstring& col, DBColumnSchema& colSchema)
{
	for (ColumnCollection::iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
	{
		if(col == iter->DBName)
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

bool CDBTableSchema::SetColumn(const DBColumnSchema& col)
{
	if(col.Index < Columns.size())
	{
		Columns[col.Index] = col;
		return true;
	}

	return false;
}

bool CDBTableSchema::AddColumn(DBColumnSchema& col)
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

bool CDBTableSchema::BuildInSchemaValidate()
{
	bool bValid = false;

	do 
	{
		if(DBName.empty()) break;

		ColumnCollection::iterator iter;
		for (iter = Columns.begin(); iter != Columns.end(); ++iter)
		{
			if(iter->DBName.empty() || !iter->DBType || iter->DBType->CompatibleWith(iter->Type))
			{
				break;
			}
		}

		if(iter != Columns.end()) break;

		bValid = true;

	} while (false);
	
	return bValid;
}

void CDBTableSchema::Clear()
{
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
		iter->Flag = DBColumnSchema::BuildIn;
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
		if(!FindByDBName(dbCol.DBName, col))
		{
			col.Name = dbCol.DBName;			
			Columns.push_back(dbCol);
		}

		dbCol.SetFlag(DBColumnSchema::BuildIn, false);
		dbCol.SetFlag(DBColumnSchema::KeyColumn, false);

		col.Type = dbCol.DBType->PreferredCppDataType();
		col.Flag |= DBExist | dbCol.Flag;
		SetColumn(col);
	}

	//
}