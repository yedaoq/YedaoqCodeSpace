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
}

CDBModule::DBTableEnumerator CDBModule::EnumTable() const
{
	return make_iterator_enumerator(Tables_.begin(), Tables_.end());
}

int	CDBModule::Clear()
{
	DBTableCollection::iterator iter = Tables_.begin();
	while ( iter != Tables_.end())
	{
		if(!(*iter)->GetSchema().IsBuildin())
		{
			break;
		}
	}
	
	if(iter != Tables_.end())
	{
		std::for_each(iter, Tables_.end(), std::ptr_fun(
	}
	
}

int	CDBModule::RefreshSchema()
{
	
}

int	CDBModule::ClearData()
{

}

int	CDBModule::RefreshData()
{

}