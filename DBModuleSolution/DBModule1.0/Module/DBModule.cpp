#include "DBModule.h"
#include "DBDataAdapter.h"
#include "Enumerator.h"
#include "DBTable.h"
#include "DBFactory.h"
#include <algorithm>
#include "Schema\DBSchemaValidater.h"
#include "..\DBCommon\DBNameMappingNone.h"

using namespace NSDBModule;

int	CDBModule::Initialize()
{
	int iRet = 0;
	
	do
	{
		if(iRet = InitializeBuildinSchema() <= 0) break;

	}while(false);
	
	return 1;
}

int CDBModule::AttachToDatabase(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory, IDBNameMapping* dbNameMapping)
{
	if(!dbAdapter || !dbFactory)
	{
		// invalid argument
		_ASSERT(false);
		return -1;
	}

	if(BindingToAnyDataBase()) // detach from origin database at first
	{
		if(DetachFromDataBase() <= 0)
		{
			return -1;
		}
	}

	DBAdapter_ = dbAdapter;
	DBFactory_ = dbFactory;
	DBNameMapping_ = dbNameMapping;

	if(!DBNameMapping_)	// if no db name mapper, use the default
	{
		DBNameMapping_ = new CDBNameMappingNone;
	}
	
	return RefreshSchema();
}

int CDBModule::DetachFromDataBase()
{
	// deal with the uncommit data as so on

	// reset db interface
	delete DBAdapter_;
	DBAdapter_ = 0;
	delete DBFactory_;
	DBFactory_ = 0;
	delete DBNameMapping_;
	DBNameMapping_ = 0;

	// reset dbschema validity
	SchemaValidity_ = Unknow;

	return 1;
}

int	CDBModule::Clear(bool bBuildIn)
{
	Tables().Clear(bBuildIn);

	return 1;
}

int	CDBModule::RefreshSchema()
{
	Clear();

	CDBSchemaLoader loader(this);
	
	if(loader.Load())
	{
		return -1;
	}

	CDBSchemaValidater validater(*this);

	SchemaValidity_ = validater.Validate();

	return 1;
}

int	CDBModule::ClearData()
{
	return 0;
}

int	CDBModule::RefreshData()
{
	return 0;
}