#include "DBTableCollection.h"
#include <crtdbg.h>
#include <exception>
#include "DBTable.h"
#include "DBTableSchema.h"

using namespace NSDBModule;

const CDBTable* CDBTableCollection::operator[](const tstring& tbl) const
{
	if(tbl.empty())
	{
		_ASSERT(false);
		return 0;
	}

	for (DBTableCollection::const_iterator iter = Tables.begin(); iter != Tables.end(); ++iter)
	{
		if((*iter)->GetName() == tbl)
		{
			return *iter;
		}
	}

	return 0;
}

const CDBTable* CDBTableCollection::operator[](index_t tbl) const
{
	if(tbl < 0 || tbl >= Tables.size())
	{
		_ASSERT(false);
		throw std::exception();
		return 0;
	}

	return Tables[tbl];
}

DBTableEnumerator CDBTableCollection::Enum() const
{
	return make_iterator_enumerator(Tables.begin(), Tables.end());
}

CDBTable* CDBTableCollection::Append(const tstring& name, CDBModule* module, bool bBuildIn )
{
	DBTablePtr ptrRet = 0;
	if(name.empty() || !module)
	{
		_ASSERT(false);
		return ptrRet;
	}

	CDBTable* pTbl = new CDBTable(module);
	CDBTableSchema& schema = pTbl->GetSchema();
	schema.Name = name;
	schema.SetFlag(CDBTableSchema::BuildIn, bBuildIn);

	Tables.push_back(DBTablePtr(pTbl));
	ptrRet = Tables[Tables.size() - 1];

	return ptrRet;
}

CDBTable* CDBTableCollection::Append(const CDBTableSchema& schema, CDBModule* module, bool bBuildIn)
{
	DBTablePtr ptrRet = 0;
	if(schema.Name.empty() || !module)
	{
		_ASSERT(false);
		return ptrRet;
	}

	CDBTable* pTbl = new CDBTable(module, schema);
	pTbl->GetSchema().SetFlag(CDBTableSchema::BuildIn, bBuildIn);

	Tables.push_back(DBTablePtr(pTbl));
	ptrRet = Tables[Tables.size() - 1];
	return ptrRet;
}

int CDBTableCollection::Remove(const tstring& name)
{
	if(name.empty())
	{
		_ASSERT(false);
		return 0;
	}

	DBTableCollection::iterator iter = Tables.begin();
	while(iter != Tables.end())
	{
		if((*iter)->GetName() == name)
		{
			break;
		}
	}

	if(iter != Tables.end())
	{
		ReleaseTables(iter, Tables.end());
		Tables.erase(iter,iter);
		return 1;
	}

	return 0;	
}

int CDBTableCollection::Remove(index_t tbl)
{
	if(tbl < 0 || tbl >= Tables.size())
	{
		_ASSERT(false);
		return 0;
	}

	DBTableCollection::iterator iter = Tables.begin() + tbl;
	ReleaseTables(iter, iter + 1);
	Tables.erase(iter,iter);
	return 1;
}

int	CDBTableCollection::Clear(bool bBuildIn)
{
	// get first item to remove
	DBTableCollection::iterator iter = Tables.begin();
	if(!bBuildIn)
	{
		while ( iter != Tables.end())
		{
			if(!(*iter)->GetSchema().IsBuildin())
			{
				break;
			}
		}		
	}
	
	// release memory of all item to be removed
	ReleaseTables(iter, Tables.end());

	// remove item from list
	Tables.erase(iter, Tables.end());

	// clear data and extern schema of all remaining tables
	for (DBTableCollection::iterator iter = Tables.begin(); iter != Tables.end(); ++iter)
	{
		(*iter)->GetSchema().Clear(false);
		(*iter)->ClearData();
	}

	return 1;
}

void CDBTableCollection::ReleaseTables(DBTableCollection::iterator begin, DBTableCollection::iterator end)
{
	for (DBTableCollection::iterator iter = begin; iter != end; ++iter)
	{
		delete *iter;
		*iter = 0;
	}
}