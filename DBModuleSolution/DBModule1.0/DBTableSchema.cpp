#include "DBTableSchema.h"
#include <stdexcept>

using namespace NSDBModule;

DBColumnSchema& CDBTableSchema::operator[](const tstring& col)
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

DBColumnSchema& CDBTableSchema::operator[](index_t col)
{
	if(col < ColumnSchema.size())
	{
		_ASSERT(ColumnSchema[col].Index == col);
		return ColumnSchema[col];
	}

	throw std::out_of_range("");
}

//CDBTableSchema::ColumnSchemaEnumPtr CDBTableSchema::GetColumnSchemaEnumerator()
//{
//	return CIteratorEnumerator<DBColumnSchema>(ColumnSchema.begin(), ColumnSchema.end());
//}