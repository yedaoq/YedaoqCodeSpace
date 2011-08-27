#include "DBTableCollection.h"
#include <crtdbg.h>
#include <exception>
#include "DBTable.h"
#include "DBTableSchema.h"

using namespace NSDBModule;

DBTablePtr_const CDBTableCollection::operator[](const tstring& tbl) const
{
	if(tbl.empty())
	{
		_ASSERT(false);
		return 0;
	}

	for (DBTableCollection::const_iterator iter = Tables.begin(); iter != Tables.end(); ++iter)
	{
		if(((DBTablePtr)(*iter))->GetName() == tbl)
		{
			return (DBTablePtr)(*iter);
		}
	}

	return 0;
}

DBTablePtr_const CDBTableCollection::operator[](index_t tbl) const
{
	if(tbl < 0 || tbl >= Tables.size())
	{
		_ASSERT(false);
		throw std::exception();
		return 0;
	}

	return (DBTablePtr)Tables[tbl];
}

DBTableEnumerator CDBTableCollection::Enum() const
{
	//return make_iterator_enumerator(Tables_.begin(), Tables_.end());
	return make_iterator_enumerator(Tables.begin(), Tables.end());
}

DBTablePtr CDBTableCollection::Append(const tstring& name, CDBModule* module, bool bBuildIn )
{
	if(name.empty() || !module)
	{
		_ASSERT(false);
		return 0;
	}

	CDBTable* pTbl = new CDBTable(module);
	CDBTableSchema& schema = pTbl->GetSchema();
	schema.Name = name;
	schema.SetFlag(CDBTableSchema::BuildIn, bBuildIn);

	Tables.push_back(DBTablePtr(pTbl));
	return (DBTablePtr)Tables[Tables.size() - 1];
}

DBTablePtr CDBTableCollection::Append(const CDBTableSchema& schema, CDBModule* module, bool bBuildIn)
{
	if(schema.Name.empty() || !module)
	{
		_ASSERT(false);
		return 0;
	}

	CDBTable* pTbl = new CDBTable(module, schema);
	pTbl->GetSchema().SetFlag(CDBTableSchema::BuildIn, bBuildIn);

	Tables.push_back(DBTablePtr(pTbl));
	return (DBTablePtr)Tables[Tables.size() - 1];
}

DBTablePtr CDBTableCollection::Remove(const tstring& name)
{
	int idx = 0;
	for (DBTableCollection::const_iterator iter = Tables.begin(); iter != Tables.end(); ++iter)
	{
		if(((DBTablePtr)(*iter))->GetName() == name)
		{
			return Remove(idx);
		}
		++idx;
	}
}

DBTablePtr CDBTableCollection::Remove(index_t tbl)
{
	DBTableCollection::iterator iter = Tables.begin() + tbl;
	DBTablePtr pTbl = *iter;
	Tables.erase(iter,iter);
	return pTbl;
}

int	CDBTableCollection::Clear(bool bBuildIn)
{
	DBTableCollection::iterator iter = Tables.begin();
	if(!bBuildIn)
	{
		while ( iter != Tables.end())
		{
			if(!((DBTablePtr)(*iter))->GetSchema().IsBuildin())
			{
				break;
			}
		}		
	}
	
	Tables.erase(iter, Tables.end());

	for (iter = Tables.begin(); iter != Tables.end(); ++iter)
	{
		((DBTablePtr)(*iter))->GetSchema().Clear();
	}

	return 1;
}