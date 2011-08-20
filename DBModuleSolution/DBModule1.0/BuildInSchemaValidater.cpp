#include "BuildInSchemaValidater.h"
#include "DBModule.h"
#include "DBColumnSchema.h"
#include "DBDataType.h"

using namespace NSDBModule;

bool CBuildInSchemaValidater::Invalidate()
{
	bool bRet = true;

	IEnumerator<CDBTable>& etor = DBModule.EnumTable();
	while (etor.MoveNext())
	{
		if(ValidateTableSchema(etor.Current().GetSchema()))
		{
			bRet = false;
		}
	}

	return bRet;
}

bool CBuildInSchemaValidater::ValidateTableSchema(const CDBTableSchema& tbl)
{
	bool bRet = true;

	if(tbl.IsBuildin() && !tbl.IsDBExist())
	{		
		return false;
	}

	IEnumerator<DBColumnSchema>& etor = tbl.EnumColumn();
	while(etor.MoveNext())
	{
		if(!ValidateColumnSchema(etor.Current()))
		{
			bRet = false;
		}
	}

	return bRet;
}

bool CBuildInSchemaValidater::ValidateColumnSchema(const DBColumnSchema& col)
{
	bool bRet = false;
	do 
	{
		if(col.IsBuildin() && !col.IsDBExist()) break;
		if(!col.IsBuildin() && col.IsDBExist())
		{
			if(col.IsDBPrimaryKey() || !col.IsDBNullable()) break;
		}

		if(EnumCppDataType::CppUnknow == col.Type) break;
		if(!col.DBType) break;
		if(!col.DBType->CompatibleWith(col.Type)) break;

		if(col.Name.empty()) break;
		if(col.DBName.empty()) break;
		if(col.DBIndex < 0) break;

		bRet = true;

	} while (false);
	
}