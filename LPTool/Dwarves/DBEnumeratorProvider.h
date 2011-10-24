#pragma once
#include <Util\DBEnumeratorSuit.h>
#include <Singlton.h>
#include <map>
#include "DBModuleLP.h"
#include <Module\DBModule.h>
#include <utility>
#include <Module\DBTable.h>

using namespace NSDBModule;

class CDBEnumeratorProvider : public CSingleton<CDBEnumeratorProvider>
{
public:
	typedef std::map<int, CDBEnumeratorSuit> SuitMap;

public:
	CDBEnumeratorProvider()
		: Module_(&DBModule)
	{}

	CDBEnumeratorProvider(CDBModule* module)
		: Module_(module)
	{}

	CDBEnumeratorSuit& operator[](int tbl)
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

protected:
	SuitMap		Suits_;
	CDBModule*	Module_;
};
