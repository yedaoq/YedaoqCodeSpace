#include "DBSchemaLoader.h"
#include "DBModule.h"
#include "DBDataAdapter.h"
#include "DBNameMapping.h"
#include "DBTableCollection.h"
#include "DBTable.h"
#include "DBColumnSchema.h"
#include "mytype.h"
#include "DBFactory.h"
#include "DBDataType.h"

using namespace NSDBModule;

bool CDBSchemaLoader::Load()
{
	bool bRet = true;

	if(!DBModule->BindingToAnyDataBase())
	{
		return false;
	}

	if(!LoadAllTable())
	{
		bRet = false;
	}

	return bRet;
}

bool CDBSchemaLoader::LoadAllTable()
{
	bool bRet;

	DBModule->Tables().Clear();

	IDBDataAdapter::DBTableEnumPtr etor = DBModule->DBAdapter()->EnumTable();
	if(!etor)
	{
		return false;
	}

	ContextDBNameMapping context = {EnumDBNameCategory::Table, 0};
	tstring dbName;
	tstring name;
	while (etor->MoveNext(dbName))
	{
		name = DBModule->DBNameMapping()->FromDBName(dbName, context);
		if(name.empty())
		{
			continue;
		}

		DBTablePtr pTable = DBModule->Tables()[name];
		if(!pTable)
		{
			pTable = DBModule->Tables().Append(name, DBModule, false);
		}

		if(!LoadTable(dbName, pTable))
		{
			bRet = false;
		}
	}

	return bRet;
}

bool CDBSchemaLoader::LoadTable(const tstring& dbName, const DBTablePtr& pTbl)
{
	CDBTableSchema& schema = pTbl->GetSchema();

	schema.Clear();
	schema.DBName = dbName;

	IDBDataAdapter::DBColumnEnumPtr etor = DBModule->DBAdapter()->EnumColumn(dbName);
	if(!etor)
	{
		return false;
	}

	schema.SetFlag(CDBTableSchema::DBExist, true);

	DBColumnSchema dbCol; // column info from db
	DBColumnSchema exCol; // exist column info
	ContextDBNameMapping context = { EnumDBNameCategory::Column, &schema };

	while(etor->MoveNext(dbCol))
	{
		dbCol.Name = DBModule->DBNameMapping()->FromDBName(dbCol.DBName, context);
		if(!schema.FindByName(dbCol.Name, exCol))
		{
			exCol.SetBuildInInfo(dbCol.Name, dbCol.DBType->PreferredCppDataType(), false, false);
			exCol.ResetExternInfo();
			schema.AppendColumn(exCol);
		}
		exCol.SetExternInfo(dbCol.DBName, dbCol.DBType, true, dbCol.IsDBPrimaryKey(), dbCol.IsDBNullable());
		schema.ModifyColumn(exCol);
	}

}