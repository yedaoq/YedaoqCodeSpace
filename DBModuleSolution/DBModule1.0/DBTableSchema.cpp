#include "DBTableSchema.h"
#include <stdexcept>

const DBColumnSchema& CDBTableSchema::operator[](const tstring& col)
{
	for (DBColumnSchemaVct::iterator iter = ColumnSchema.begin(); iter != ColumnSchema.end(); ++iter)
	{
		if(col == iter->Name)
		{
			return *iter;
		}
	}

	throw std::out_of_range("");
}

const DBColumnSchema& CDBTableSchema::operator[](index_t col)
{
	if(col < ColumnSchema.size())
	{
		return ColumnSchema[col];
	}

	throw std::out_of_range("");
}

CDBTableSchema::ColumnSchemaEnumPtr CDBTableSchema::GetColumnSchemaEnumerator()
{
	return ColumnSchemaEnumPtr(new CIteratorEnumerator<DBColumnSchema>(ColumnSchema.begin(), ColumnSchema.end()));
}