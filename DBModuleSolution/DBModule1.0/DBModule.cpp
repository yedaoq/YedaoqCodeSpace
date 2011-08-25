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
	//return make_iterator_enumerator(Tables_.begin(), Tables_.end());
}

int	CDBModule::Clear()
{
	DBTableCollection::iterator iter = Tables_.begin();
	if(WithBuildinSchema())
	{
		while ( iter != Tables_.end())
		{
			if(!((DBTablePtr)(*iter))->GetSchema().IsBuildin())
			{
				break;
			}
		}
	}
	
	Tables_.erase(iter, Tables_.end());
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