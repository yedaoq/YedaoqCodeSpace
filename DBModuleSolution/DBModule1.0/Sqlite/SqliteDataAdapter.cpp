#include "SqliteDataAdapter.h"
#include "SqliteDataTypeProvider.h"
#include "..\Module\Schema\DBColumnSchema.h"
#include "..\DBInterface\DBSourcePath.h"
#include "..\DBInterface\DBCommand.h"
#include "..\Module\DBRecord.h"
#include "CppSQLite3U.h"
#include "I_DBControl.h"
#include "boost\smart_ptr\make_shared.hpp"

using namespace NSDBModule;

CSqliteDataAdapter::CSqliteDataAdapter(IDBConnection* conn)
{
	if(!SqliteDB.open(conn->ToString().c_str()))
	{
		throw std::exception();
	}
}

IDBDataAdapter::DBTableEnumPtr CSqliteDataAdapter::EnumTable()
{
	I_CppSQLite3Query* spQuery = SqliteDB.execQuery2(TEXT("SELECT name FROM [sqlite_master] WHERE type = 'table' and name not in ('sqlite_sequence')"));
	return boost::make_shared<CSqliteTableEnumerator>(spQuery);	
}

IDBDataAdapter::DBColumnEnumPtr CSqliteDataAdapter::EnumColumn(const tstring& tblName)
{
	tchar buf[280] = TEXT("SELECT * FROM '\0");
	int len = _tcslen(buf);
	tblName.copy(buf + len, tblName.length());
	buf[len + tblName.length()] = '\'';
	buf[len + tblName.length() + 1] = '\0';

	I_CppSQLite3Query* spQuery = SqliteDB.execQuery2(buf);
	return boost::make_shared<CSqliteColumnEnumerator>(spQuery);
}

IDBDataAdapter::DBRecordEnumPtr CSqliteDataAdapter::Select(const IDBCommand& cmd)
{
	if(EnumDBCommandType::Sql != cmd.Type())
	{
		throw std::exception();
	}

	I_CppSQLite3Query* spQuery = SqliteDB.execQuery2(cmd.Text().c_str());
	return boost::make_shared<CSqliteRecordEnumerator>(spQuery);
}

int	CSqliteDataAdapter::Execute(const IDBCommand& cmd)
{
	if(EnumDBCommandType::Sql != cmd.Type())
	{
		throw std::exception();
	}

	return SqliteDB.execScalar(cmd.Text().c_str());
}

bool CSqliteTableEnumerator::MoveNext()
{
	if(BeforeFirst)
	{
		BeforeFirst = false;
	}
	else
	{
		if(!(QueryPtr->eof()))
		{
			QueryPtr->nextRow();
		}
	}

	return !(QueryPtr->eof());
}

const tstring& CSqliteTableEnumerator::Current()
{
	static tstring str;
	if(BeforeFirst || !QueryPtr || QueryPtr->eof())
	{
		throw std::exception();
	}
	str = QueryPtr->getStringField(0);
	return str;
}

bool CSqliteTableEnumerator::MoveNext(tstring& tbl)
{
	if(MoveNext())
	{
		tbl = Current();
		return true;
	}

	return false;
}

bool CSqliteColumnEnumerator::MoveNext()
{
	if(IdxCol < QueryPtr->numFields())
	{
		++IdxCol;
	}
	return IdxCol < QueryPtr->numFields();
}

const DBColumnSchema& CSqliteColumnEnumerator::Current()
{
	static DBColumnSchema col = {0, TEXT(""), EnumCppDataType::CppUnknow, 0, TEXT(""), 0, 0, 0 };
	if(!QueryPtr || IdxCol < 0 || IdxCol >= QueryPtr->numFields())
	{
		throw std::exception();
	}

	col.DBIndex = IdxCol;
	col.DBName = QueryPtr->fieldName(IdxCol);
	col.DBType = CSqliteDataTypeProvider::GetInstance().ParseDBTypeStr(QueryPtr->fieldDeclType(IdxCol));

	return col;
}

bool CSqliteColumnEnumerator::MoveNext(DBColumnSchema& tbl)
{
	if(MoveNext())
	{
		tbl = Current();
		return true;
	}

	return false;
}

bool CSqliteRecordEnumerator::MoveNext()
{
	if(BeforeFirst)
	{
		BeforeFirst = false;
	}
	else
	{
		if(!QueryPtr->eof())
		{
			QueryPtr->nextRow();
		}
	}

	return !QueryPtr->eof();
}

const IDBRecord& CSqliteRecordEnumerator::Current()
{
	if(BeforeFirst || !QueryPtr || QueryPtr->eof())
	{
		throw std::exception();
	}
	return Record;
}

bool CSqliteRecordEnumerator::MoveNext(IDBRecord& rec)
{
	bool bResult = MoveNext();
	if(bResult)
	{
		if(rec.GetFieldCount() != Current().GetFieldCount())
		{
			throw std::exception();
		}

		for (index_t i = 0; i < rec.GetFieldCount(); ++i)
		{
			rec.SetField(i, Current().GetField(i));
		}
	}

	return false;
}