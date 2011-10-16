#include "DBModule.h"
#include "Enumerator.h"
#include "DBTable.h"
#include "..\DBInterface\DBDataAdapter.h"
#include "..\DBInterface\DBFactory.h"
#include "Schema\DBSchemaValidater.h"
#include "..\DBCommon\DBNameMappingNone.h"
#include <algorithm>

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
	
	if(1 != RefreshSchema())
	{
		_ASSERT(false);
		return -1;
	}

	std::auto_ptr<DBTableEnumerator> pEnumTbl(Tables().Enum());

	if(pEnumTbl.get())
	{
		while(pEnumTbl->MoveNext())
		{
			pEnumTbl->Current()->initializeAccess();
		}
	}

	return 1;
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
	
	if(loader.Load() != 1)
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