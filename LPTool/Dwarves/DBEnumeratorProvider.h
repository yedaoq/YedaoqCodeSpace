#pragma once
#include <Util\DBEnumeratorSuit.h>

class CDBEnumeratorProvider : CSingleton<CDBEnumeratorProvider>
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
		if(!Module_ || tbl < 0 || tbl >= Module_.Tables().Count())
		{
			_ASSERT(false);
			throw std::exception();
		}

		SuitMap::iterator iter = Suits_.find(tbl);
		if (iter == Suits_.end())
		{
			std::auto_ptr<DBRecordEnumerator> pEnumRec(Module_->Tables()[tbl]->EnumRecord());
			return Suits_.insert(make_pair(tbl, CDBEnumeratorSuit(*pEnumRec))).second;
		}
		return iter->second;
	}

protected:
	SuitMap		Suits_;
	CDBModule*	Module_;
};
