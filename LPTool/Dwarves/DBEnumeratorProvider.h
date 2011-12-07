#pragma once
#include "Util\DBEnumeratorSuit.h"
#include <Singlton.h>
#include <map>

using namespace NSDBModule;

class NSDBModule::CDBModule;
class NSDBModule::DMLEvent;

class CDBEnumeratorProvider : public CSingleton<CDBEnumeratorProvider>
{
public:
	typedef std::map<int, CDBEnumeratorSuit> SuitMap;

public:
	CDBEnumeratorProvider();
	CDBEnumeratorProvider(CDBModule* module);

	CDBEnumeratorSuit& operator[](int tbl);

	void DmlRing(DMLEvent* e);

protected:
	SuitMap		Suits_;
	CDBModule*	Module_;
};
