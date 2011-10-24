#pragma once

#include "DwarfViewInfoDBTblBase.h"

template<int TblID>
class CDwarfViewInfoDBTable : CDwarfViewInfoDBTblBase
{
public:
	CDwarfViewInfoDBTable()
		: CDwarfViewInfoDBTblBase(DBModule, TblID)
	{}
};