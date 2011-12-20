#pragma once
#include "DwarfViewInfoDBTable.h"
#include "LocalPlayerObjEnum.h"

template<>
class CDwarfViewInfoDBTable<TBL_PackageInfo> : public CDwarfViewInfoDBTblBase
{
public:
	CDwarfViewInfoDBTable()
		: CDwarfViewInfoDBTblBase(&CDBModuleLP::GetInstance(), TBL_PackageInfo)
	{}

	void MakePackage( const tstring& pckID , const tstring& pckFileName) ;

	virtual int InitializeOperations();
	afx_msg void OnMakePkg(DwarfViewOperationContext* pCtx); 
};
