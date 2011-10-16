#include "stdafx.h"
#include "Sqlite/SqliteDataAdapter.h"
#include "Sqlite/SqliteFactory.h"
#include "Sqlite/SqliteSource.h"
#include "DBNameMappingLP.h"

using namespace NSDBModule;

extern "C" __declspec(dllexport) IDBSourceManager* GetDBSourceManager()
{
	return new CSqliteSourceManager();
}

extern "C" __declspec(dllexport) IDBNameMapping* GetDBNameMapping()
{
	return new CDBNameMappingLP();
}