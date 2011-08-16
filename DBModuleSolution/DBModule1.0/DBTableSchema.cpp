#include "DBTableSchema.h"
#include <stdexcept>

using namespace NSDBModule;

bool CDBTableSchema::find(const tstring& dbCol, DBColumnSchema** colSchema)
{
	for (DBColumnSchemaCollection::iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
	{
		if(dbCol == iter->DBName)
		{
			*colSchema = &(*iter);
			return true;
		}
	}

	*colSchema = 0;
	return false;
}

bool CDBTableSchema::find(index_t colIdx, DBColumnSchema** colSchema)
{
	if(colIdx < Columns.size())
	{
		_ASSERT(Columns[colIdx].Index == colIdx);
		colSchema = &Columns[colIdx];
		return true;
	}
	return false;
}

DBColumnSchema& CDBTableSchema::operator[](const tstring& dbCol)
{
	for (DBColumnSchemaCollection::iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
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

bool CDBTableSchema::Validate(IDBDataAdapter* dbAdapter)
{
	bool bValid = false;

	do 
	{
		if(DBName.empty()) break;

		DBColumnSchemaCollection::iterator iter;
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

	// clear the DBExist flag of all column
	for(DBColumnSchemaCollection::iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
	{
		iter->SetFlag(DBColumnSchema::DBExist, false);
	}
}

bool CDBTableSchema::Load(IDBDataAdapter* dbAdapter)
{
	Clear();

	// get database columns
	DBColumnEnumPtr pEnum = dbAdapter->EnumColumn(this->DBName);
	if(!pEnum.get())
	{
		//assert
		return false;
	}

	// merge columns schema
	DBColumnSchema dbCol;
	pEnum->Reset();
	while(pEnum->MoveNext(dbCol))
	{
		DBColumnSchema* col;
		if(!this->find(dbCol.DBName, &col))
		{
			dbCol.Index = Columns.size();
			dbCol.Name = dbCol.DBName;
			dbCol.Type = dbCol.DBType->PreferredCppDataType();
			Columns.push_back(dbCol);
			col = &Columns[Columns.size() - 1];
		}

		col->SetFlag(DBColumnSchema::DBExist, true);
		col->SetFlag(DBColumnSchema::DBPrimaryKey, dbCol.IsPrimaryKey());
		col->SetFlag(DBColumnSchema::DBNullable, dbCol.IsNullable());
		col->SetFlag(DBColumnSchema::DBUnique, dbCol.IsUnique());
	}

	//
}