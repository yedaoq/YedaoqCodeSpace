#include "StdAfx.h"
#include "DBModuleLP.h"
#include "Module/Schema/DBSchemaCppMicro.h"
#include <memory>
#include "DBNameMappingLP.h"
#include "DBInterface/DBSourcePath.h"
#include "DBInterface/DBDataAdapter.h"
#include "DBInterface/DBFactory.h"
#include "Sqlite/SqliteSource.h"

using namespace NSDBModule;

int	CDBModuleLP::InitializeBuildinSchema()
{
	index_t anonyousTableIndex = 0;
	CDBTableSchema anonyousTableSchema;
	#include "LocalPlayer.dbschema"

	return CDBModule::InitializeBuildinSchema();
}

CDBModuleLP DBModule;

bool DBInitialize()
{
	CSqliteSourceManager smr;
	std::auto_ptr<IDBConnection> pConn(smr.PromptSelectDBSource(0));
	IDBDataAdapter* PAdapter;
	IDBFactory*		pFactory;
	IDBNameMapping* pMapping = new CDBNameMappingLP();

	if(pConn.get() && smr.OpenDBConnection(pConn.get(), &PAdapter, &pFactory))
	{
		DBModule.AttachToDatabase(PAdapter, pFactory, pMapping);
		return true;
	}

	return false;
}
