#include "DBTableSchema.h"
#include "..\..\DBInterface\DBDataAdapter.h"
#include "..\..\DBInterface\DBDataType.h"
#include <stdexcept>
#include <algorithm>

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

bool CDBTableSchema::RemoveColumn(index_t col)
{
	if(col < 0 || col >= Columns.size())
	{
		_ASSERT(false);
		return false;
	}

	ColumnCollection::iterator iter = Columns.begin() + col;
	iter = Columns.erase(iter, iter);

	while(iter != Columns.end())
	{
		--(iter->Index);
		++iter;
	}
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

void CDBTableSchema::Clear(bool bBuildIn)
{
	DBName.clear();
	SetFlag(DBExist, false);

	if(bBuildIn)
	{
		Columns.clear();
	}
	{
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
		}
	}
	
}