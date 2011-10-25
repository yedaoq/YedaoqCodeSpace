#pragma once

#include "DwarfViewInfoDBTblBase.h"

template<int TblID>
class CDwarfViewInfoDBTable : public CDwarfViewInfoDBTblBase
{
public:
	CDwarfViewInfoDBTable()
		: CDwarfViewInfoDBTblBase(DBModule, TblID)
	{}
};