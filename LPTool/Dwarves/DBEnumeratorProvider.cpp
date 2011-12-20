#include "stdafx.h"
#include "DBEnumeratorProvider.h"
#include <utility>
#include "DBModuleLP.h"
#include "Module\DBModule.h"
#include "Module\DBTable.h"
#include "Module\DMLNotifier\DMLEvent.h"

CDBEnumeratorProvider::CDBEnumeratorProvider()
	: Module_(&CDBModuleLP::GetInstance())
{
	Module_->DMLNotifier().AppendMonitor(
		EnumDMLCommand::DMLDelete | EnumDMLCommand::DMLInsert | EnumDMLCommand::DMLUpdate,
		-1,
		DelegateDMLMonit(this, &CDBEnumeratorProvider::DmlRing));
}

CDBEnumeratorProvider::CDBEnumeratorProvider(CDBModule* module)
	: Module_(module)
{}

CDBEnumeratorSuit& CDBEnumeratorProvider::operator[](int tbl)
{
	if(!Module_ || tbl < 0 || tbl >= Module_->Tables().Count())
	{
		_ASSERT(false);
		throw std::exception();
	}

	SuitMap::iterator iter = Suits_.find(tbl);
	if (iter == Suits_.end())
	{
		std::auto_ptr<DBRecordEnumerator> pEnumRec(Module_->Tables()[tbl]->EnumRecord());
		iter = Suits_.insert(std::make_pair(tbl, CDBEnumeratorSuit(*pEnumRec))).first;
	}
	return iter->second;
}

void CDBEnumeratorProvider::DmlRing( DMLEvent* e )
{
	if(e && e->TableID >= 0)
	{
		/*SuitMap::iterator iter = Suits_.find(e->TableID);
		if(iter != Suits_.end())
		{
			std::auto_ptr<DBRecordEnumerator> pEnumRec(Module_->Tables()[e->TableID]->EnumRecord());
			iter->second = *pEnumRec;
		}*/

		Suits_.erase(e->TableID);
	}
}


