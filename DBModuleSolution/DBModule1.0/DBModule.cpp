#include "DBModule.h"
#include "DBDataAdapter.h"
#include "Enumerator.h"
#include "DBTable.h"
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

int CDBModule::AttachToDatabase(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory)
{
	if(!dbAdapter || !dbFactory)
	{
		// invalid argument
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
	
	RefreshSchema();
}

int CDBModule::DetachFromDataBase()
{
	// deal with the uncommit data as so on
	DBAdapter_ = 0;
	DBFactory_ = 0;
	SchemaValidity_ = Unknow;

	return 1;
}

int	CDBModule::Clear()
{
	Tables().Clear(false);

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