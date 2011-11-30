#pragma once

#include "DwarfViewInfoDBTblBase.h"
#include "DBModuleLP.h"

template<int TblID>
class CDwarfViewInfoDBTable : public CDwarfViewInfoDBTblBase
{
public:
	CDwarfViewInfoDBTable()
		: CDwarfViewInfoDBTblBase(&g_DBModule, TblID)
	{}

};