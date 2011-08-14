#include "DBCommandBuilder.h"
#include "DBTableSchema.h"
#include <crtdbg.h>
#include "DBCommand.h"
#include <winnt.h>
#include "DBRecord.h"
#include "DBDataType.h"
#include <tchar.h>

using namespace NSDBModule;

int CDBCommandBuilderBase::Initialize(const CDBTableSchema* schema)
{
	bool bSchemaLegal = false;

	do 
	{
		if(!schema) break;
		if(schema->DBName.empty()) break;
		if(schema->ColumnSchema.size()  == 0) break;

		const int fieldCount = schema->ColumnSchema.size();
		int i = 0;
		for (; i < fieldCount; ++i)
		{
			const DBColumnSchema& col = (*schema)[i];
			if(col.DBName.empty() || !col.DBType || col.Name.empty() || col.Type == CppUnknow) break;
		}
		
		if(fieldCount != i) break;

		bSchemaLegal = true;

	} while (false);
	
	if(!bSchemaLegal)
	{
		throw std::exception();
		return 0;
	}

	TableSchema_ = schema;

	return 1;

#ifdef PREGENERATESQLFORFAST
	tstring				SqlSelectHead;
	tstring				SqlUpdateHead;
	tstring				SqlInsertHead;
	tstring				SqlDeleteHead;
	tstring				SqlCreateHead;
#endif
}

int CDBCommandBuilderBase::GenerateDBCommand(IDBCommand** cmd, EnumDBCommandType type, const tstring& text)
{
	*cmd = new CDBCommandBase(text, type);
}

int CDBCommandBuilderBase::GetCmdSelect(const CDBRecordBase& rec, const CDBRecordComparison& cmp, IDBCommand** cmd)
{
	tstring strSql;
	GenerateSqlSelect(rec, cmp, strSql);
	GenerateDBCommand(cmd, Sql, strSql);
	return 1;
}

int CDBCommandBuilderBase::GetCmdSelect(IDBCommand** cmd)
{
	tstring strSql;
	GenerateSqlSelect(strSql);
	GenerateDBCommand(cmd, Sql, strSql);
	return 1;
}

int CDBCommandBuilderBase::GetCmdInsert(const CDBRecordBase& rec, IDBCommand** cmd)
{
	tstring strSql;
	GenerateSqlInsert(rec, strSql);
	GenerateDBCommand(cmd, Sql, strSql);
	return 1;
}

int CDBCommandBuilderBase::GetCmdDelete(const CDBRecordBase& rec, const CDBRecordComparison& cmp, IDBCommand** cmd)
{
	tstring strSql;
	GenerateSqlDelete(rec, cmp, strSql);
	GenerateDBCommand(cmd, Sql, strSql);
	return 1;
}

int CDBCommandBuilderBase::GetCmdUpdate(const CDBRecordBase& ori, const CDBRecordBase& cur, const CDBRecordComparison& cmp, IDBCommand** cmd)
{
	tstring strSql;
	GenerateSqlUpdate(ori, cur, cmp, strSql);
	GenerateDBCommand(cmd, Sql, strSql);
	return 1;
}

int CDBCommandBuilderBase::GetCmdCreate(IDBCommand** cmd)
{
	tstring strSql;
	GenerateSqlCreate(strSql);
	GenerateDBCommand(cmd, Sql, strSql);
	return 1;
}

int CDBCommandBuilderBase::GenerateSqlSelect(const CDBRecordBase& rec , const CDBRecordComparison& cmp, tstring& buffer)
{
	buffer.append(TEXT("SELECT * FROM "));
	WrapperIdentifier(TableSchema_->DBName.c_str(), buffer);
	GenerateConditionStr(rec, cmp, buffer);
}

int CDBCommandBuilderBase::GenerateSqlSelect(tstring& buffer)
{
	buffer.append(TEXT("SELECT * FROM "));
	WrapperIdentifier(TableSchema_->DBName.c_str(), buffer);
}

int CDBCommandBuilderBase::GenerateSqlInsert(const CDBRecordBase& rec, tstring& buffer)
{
	buffer.append(TEXT("INSERT INTO "));
	WrapperIdentifier(TableSchema_->DBName.c_str(), buffer);

	buffer.append(TEXT(" ("));

	for (int i = 0; i < TableSchema_->ColumnSchema.size(); ++i)
	{
		WrapperIdentifier(TableSchema_->ColumnSchema[i].DBName.c_str(), buffer);
		buffer.append(TEXT(", "));
	}
	
	*(buffer.end() - 2) = ')';
	buffer.append(TEXT("VALUES ("));

	for (int i = 0; i < TableSchema_->ColumnSchema.size(); ++i)
	{
		WrapperValue(rec.GetField(i), TableSchema_->ColumnSchema[i].DBType, buffer);
		buffer.append(TEXT(", "));
	}

	*(buffer.end() - 2) = ')';
}

int CDBCommandBuilderBase::GenerateSqlDelete(const CDBRecordBase& rec, const CDBRecordComparison& cmp, tstring& buffer)
{
	buffer.append(TEXT("DELETE FROM "));
	WrapperIdentifier(TableSchema_->DBName.c_str(), buffer);
	GenerateConditionStr(rec, cmp, buffer);
}

int CDBCommandBuilderBase::GenerateSqlUpdate(const CDBRecordBase& ori, const CDBRecordBase& cur, const CDBRecordComparison& cmp, tstring& buffer)
{
	buffer.append(TEXT("UPDATE "));
	WrapperIdentifier(TableSchema_->DBName.c_str());
	buffer.append(TEXT(" SET "));

	for (int i = 0; i < TableSchema_->ColumnSchema.size(); ++i)
	{
		GenerateFieldAssignmentStr(TableSchema_->ColumnSchema[i], cur.GetField(i), buffer);
		buffer.append(TEXT(", "));
	}

	*(buffer.end() - 2) = ' ';
	GenerateConditionStr(ori, cmp, buffer);
}

int CDBCommandBuilderBase::GenerateSqlCreate(tstring& buffer)
{
	throw std::exception();
}

int CDBCommandBuilderBase::WrapperValue(LPCTSTR val, IDBDataType const* type, tstring& buffer)
{
	_ASSERT(type && val);
	if(type->IsNull(val))
	{
		buffer.append(TEXT("null"));
	}
	else
	{
		switch(type->Class())
		{
		case EnumDBDataTypeClass::DBString:
			{
				buffer.append(TEXT("'"));

				// convert '\'' to "''"
				PTSTR pTail = val + _tcslen(val);
				PTSTR pPosPre = val;
				PTSTR pPosCur = 0;
				while(pPosPre < pTail && (pPosCur = _tcschr(pPosPre, '\'') != NULL))
				{
					if(pPosCur - pPosPre > 1)
					{
						buffer.append(pPosPre, pPosCur);
						buffer.append(TEXT("''"));
					}

					pPosPre = pPosCur + 1;
				}

				if(pPosPre < pTail)
				{
					buffer.append(pPosPre, pTail);
				}

				buffer.append(TEXT("'"));
			}
			break;
		case EnumDBDataTypeClass::DBValue:
			{
				buffer.append(val);
			}
			break;
		case EnumDBDataTypeClass::DBBlob:
			throw std::exception();
			break;
		}
	}

	return 1;
}

int CDBCommandBuilderBase::WrapperIdentifier(LPCTSTR val, tstring& buffer)
{
	PTSTR pTail;

	if(!val || val >= (pTail = val + _tcslen(val))) // Identifier should not be null
	{
		throw new std::exception();
	}

	//buffer.append(TEXT("["));
	buffer.append(val);
	//buffer.append(TEXT("]"));
}

void CDBCommandBuilderBase::GenerateConditionStr(const CDBRecordBase& rec, const CDBRecordComparison& cmp, tstring& buffer)
{
	if(cmp.KeyFields_.size() > 0)
	{
		buffer.append(TEXT(" WHERE "));

		for (int i = 0; i < cmp.KeyFields_.size(); ++i)
		{					
			int field = cmp.KeyFields_[i];
			GenerateFieldConditionStr(TableSchema_[field], rec.GetField(field), buffer);
			buffer.append(TEXT(" AND "));
		}

		*(buffer.end() - 4) = '0';
	}
}

void CDBCommandBuilderBase::GenerateFieldAssignmentStr(const DBColumnSchema& col,const tstring& val, tstring& buffer)
{
	buffer.append(col.DBName);

	if(col.DBType->IsNull(val))
	{
		buffer.append(TEXT(" = null"));
	}
	else
	{
		buffer.append(TEXT(" = "));
		WrapperValue(val.c_str(), col.DBType, buffer);		
	}

	return 1;
}

void CDBCommandBuilderBase::GenerateFieldConditionStr(const DBColumnSchema& col,const tstring& val, tstring& buffer)
{
	buffer.append(col.DBName);

	if(col.DBType->IsNull(val))
	{
		buffer.append(TEXT(" IS null"));
	}
	else
	{
		buffer.append(TEXT(" = "));
		WrapperValue(val.c_str(), col.DBType, buffer);		
	}

	return 1;
}