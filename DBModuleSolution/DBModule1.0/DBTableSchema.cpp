#include "DBTableSchema.h"
#include <stdexcept>
#include <algorithm>
#include "DBDataAdapter.h"
#include "DBDataType.h"

using namespace NSDBModule;

bool CDBTableSchema::FindByName(const tstring& col, DBColumnSchema& colSchema) const
{
	for (ColumnCollection::const_iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
	{
		if(col == iter->Name)
		{
			colSchema = *iter;
			return true;
		}
	}
	return false;
}

bool CDBTableSchema::FindByIndex(index_t col, DBColumnSchema& colSchema) const
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

	return true;
}

const DBColumnSchema& CDBTableSchema::operator[](const tstring& dbCol) const
{
	for (ColumnCollection::const_iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
	{
		if(dbCol == iter->DBName)
		{
			return *iter;
		}
	}

	throw std::out_of_range("");
}

const DBColumnSchema& CDBTableSchema::operator[](index_t col) const
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
				std::not1(std::mem_fun_ref(&(DBColumnSchema::IsBuildin)))
			),
			Columns.end()
		);

	// reset flag of all column
	for(ColumnCollection::iterator iter = Columns.begin(); iter != Columns.end(); ++iter)
	{
		iter->ResetExternInfo();
		//iter
	}
}