#include "DBSchemaValidater.h"
#include "DBModule.h"
#include "DBColumnSchema.h"
#include "DBDataType.h"
#include "DBTable.h"

using namespace NSDBModule;

bool CDBSchemaValidater::Validate()
{
	bool bRet = true;

	std::auto_ptr<DBTableEnumerator> pEnumTbl(DBModule.Tables().Enum());
	while (pEnumTbl->MoveNext())
	{
		if(ValidateTable(pEnumTbl->Current()->GetSchema()))
		{
			bRet = false;
		}
	}

	return bRet;
}

bool CDBSchemaValidater::ValidateTable(const CDBTableSchema& tbl)
{
	if(tbl.IsBuildin())
	{
		return ValidateBuildInTable(tbl);
	}
	else
	{
		return ValidateExternTable(tbl);
	}

	return false;
}

bool CDBSchemaValidater::ValidateBuildInTable(const CDBTableSchema& tbl)
{
	if(!tbl.IsBuildin())
	{
		_ASSERT(false);
		return false;
	}

	if(!tbl.IsDBExist())
	{
		return false;
	}

	if(tbl.DBName.empty())
	{
		return false;
	}

	if(tbl.Columns.size() <= 0)
	{
		return false;
	}

	bool bRet = true;

	IEnumerator<DBColumnSchema>& etor = tbl.EnumColumn();
	while(etor.MoveNext())
	{
		if(!ValidateColumn(etor.Current()))
		{
			bRet = false;
		}
	}

	return bRet;
}

bool CDBSchemaValidater::ValidateExternTable(const CDBTableSchema& tbl)
{
	if(tbl.IsBuildin() || !tbl.IsDBExist())
	{
		_ASSERT(false);
		return false;
	}

	if(tbl.DBName.empty())
	{
		return false;
	}

	if(tbl.Columns.size() <= 0)
	{
		return false;
	}

	bool bRet = true;

	IEnumerator<DBColumnSchema>& etor = tbl.EnumColumn();
	while(etor.MoveNext())
	{
		if(!ValidateColumn(etor.Current()))
		{
			bRet = false;
		}
	}

	return bRet;
}

bool CDBSchemaValidater::ValidateColumn(const DBColumnSchema& col)
{
	if(col.IsBuildin())
	{
		return ValidateBuildInColumn(col);
	}
	else
	{
		return ValidateExternColumn(col);
	}

	return false;
}

bool CDBSchemaValidater::ValidateBuildInColumn(const DBColumnSchema& col)
{
	if(!col.IsBuildin())
	{
		_ASSERT(false);
		return false;
	}

	bool bRet = false;
	do 
	{
		if(!col.IsDBExist()) break;

		if(col.Name.empty()) break;
		if(col.DBName.empty()) break;
		if(col.DBIndex < 0) break;

		if(EnumCppDataType::CppUnknow == col.Type) break;
		if(!col.DBType) break;
		if(!col.DBType->CompatibleWith(col.Type)) break;
		if(col.IsDBPrimaryKey() || !col.IsDBNullable()) break;

		bRet = true;

	} while (false);
	
}

bool CDBSchemaValidater::ValidateExternColumn(const DBColumnSchema& col)
{
	if(col.IsBuildin() || !col.IsDBExist())
	{
		_ASSERT(false);
		return false;
	}

	bool bRet = false;
	do 
	{
		if(col.Name.empty()) break;
		if(col.DBName.empty()) break;
		if(col.DBIndex < 0) break;

		if(EnumCppDataType::CppUnknow == col.Type) break;
		if(!col.DBType) break;
		if(!col.DBType->CompatibleWith(col.Type)) break;

		bRet = true;

	} while (false);

}