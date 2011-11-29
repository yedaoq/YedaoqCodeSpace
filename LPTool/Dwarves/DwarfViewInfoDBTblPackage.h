#pragma once
#include "DwarfViewInfoDBTable.h"
#include "LocalPlayerObjEnum.h"

template<>
class CDwarfViewInfoDBTable<TBL_PackageInfo> : public CDwarfViewInfoDBTblBase
{
public:
	CDwarfViewInfoDBTable()
		: CDwarfViewInfoDBTblBase(&g_DBModule, TBL_PackageInfo)
	{}

	virtual int InitializeOperations();
	afx_msg void OnMakePkg(DwarfViewOperationContext* pCtx); 
};
