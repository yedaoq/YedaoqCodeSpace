#include "DBModule.h"
#include "DBDataAdapter.h"
#include "Enumerator.h"

using namespace NSDBModule;

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
	SchemaValidity_ = Unknow;
	Initialized_ = false;
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

int	CDBModule::ValidateSchema()
{
	if(!BindingToAnyDataBase())
	{
		return -1;
	}

	if(Unknow != SchemaValidity())
	{
		return 1;
	}

	for(DBTableCollection::iterator iter = Tables_.begin(); iter != Tables_.end(); ++iter)
	{
		if(!iter->Schema_.Validate())
		{
			return 0;
		}
	}
}

int	CDBModule::ClearSchema()
{
	for (DBTableCollection::iterator iter = Tables_.begin(); iter != Tables_.end(); ++iter)
	{
		if(WithBuildinSchema())
		{
			
		}
		else
		{

		}
	}
	

	if(WithBuildinSchema())
	{
		
	}
	else
	{

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