#include "StdAfx.h"
#include "DwarfViewInfoDBTblPackage.h"
#include <Helper.h>

int CDwarfViewInfoDBTable<TBL_PackageInfo>::InitializeOperations()
{
	__super::InitializeOperations();

	this->Operations.Append(TEXT("打包"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTable<TBL_PackageInfo>::OnMakePkg));

	return 1;
}

void CDwarfViewInfoDBTable<TBL_PackageInfo>::OnMakePkg(DwarfViewOperationContext* pCtx)
{
	CDwarfView* pMainView = CGlobalData::GetViewByID(pCtx->MainViewID);
	if(!pMainView) return 0;

	IDBRecord* pRecFocused = pMainView->GetFocusedRecord();


	//MsgboxPrompt(TEXT("未实现"));
}