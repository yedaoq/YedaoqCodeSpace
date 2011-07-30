#pragma once
#include <memory>
#include "DBColumnSchema.h"

namespace NSDBModule
{

typedef std::auto_ptr<IEnumerator<DBColumnSchema>> ColumnSchemaEnumPtr;

class CDBTableSchema
{
public:
	tstring				DBName;
	tstring				Name;
	DBColumnSchemaVct	ColumnSchema;

	const DBColumnSchema& operator[](const tstring& col);
	const DBColumnSchema& operator[](index_t col);
	ColumnSchemaEnumPtr	  GetColumnSchemaEnumerator();
};

}