#include "StdAfx.h"
#include "DwarfViewInfoDBTblPackage.h"
#include <Helper.h>
#include "DwarfOptions.h"
#include "mytype.h"
#include "fstream"
#include "iosfwd"

int CDwarfViewInfoDBTable<TBL_PackageInfo>::InitializeOperations()
{
	__super::InitializeOperations();

	this->Operations.Append(TEXT("打包"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTable<TBL_PackageInfo>::OnMakePkg));

	return 1;
}

void CDwarfViewInfoDBTable<TBL_PackageInfo>::OnMakePkg(DwarfViewOperationContext* pCtx)
{
	CDwarfView* pMainView = CGlobalData::GetViewByID(pCtx->MainViewID);
	if(!pMainView) return;

	IDBRecord* pRecFocused = pMainView->GetFocusedRecord();
	if(pRecFocused)
	{
		MakePackage(pRecFocused->GetField(COL_PackageInfo_PckId), pRecFocused->GetField(COL_PackageInfo_PckName));
		return;
	}
	
	std::auto_ptr<IEnumerator<IDBRecord>> pEnumRec(pMainView->GetSelectedRecords());
	if(!pEnumRec.get())
	{
		ASSERT(false);
		return;
	}

	while(pEnumRec->MoveNext())
	{
		MakePackage(pEnumRec->Current().GetField(COL_PackageInfo_PckId), pEnumRec->Current().GetField(COL_PackageInfo_PckName));
	}
}

void CDwarfViewInfoDBTable<TBL_PackageInfo>::MakePackage( const tstring& pckID, const tstring& pckFileName ) 
{
	TTRACE(TEXT("打包 : %s\n"), pckID.c_str());
	
	std::auto_ptr<IEnumerator<IDBRecord>> pEnumPckFile(DBModule->Tables()[TBL_PackageIndex]->FindAll(COL_PackageIndex_PckId, pckID));
	if(!pEnumPckFile.get())
	{
		ASSERT(false);
		return;
	}

	//tstring cmd = TEXT("\"C:\\Program Files\\WinRAR\WinRAR.exe\" a \"");
	

	tstring fnRarLst = TEXT("RarLst.lst");
	tchar	fpRarLst[MAX_PATH] = {0};
	CDwarfOptions::GetInstance().DirCodecFile.copy(fpRarLst, MAX_PATH);
	::PathAppend(fpRarLst, fnRarLst.c_str());

	std::locale::global(std::locale(""));
	std::tofstream fRarLst(fpRarLst, std::ios_base::out);

	CDBRecordBase recFileInfo = DBModule->Tables()[TBL_FileInfo]->RecordTemplate();
	while(pEnumPckFile->MoveNext())
	{
		if(DBModule->Tables()[TBL_FileInfo]->Find(COL_FileInfo_FileId, pEnumPckFile->Current().GetField(COL_PackageIndex_FileId), recFileInfo) > 0)
		{
			fRarLst.write(recFileInfo.GetField(COL_FileInfo_FileName).c_str(), recFileInfo.GetField(COL_FileInfo_FileName).size());
			fRarLst.write(TEXT("\n"), 1);
			//cmd.append(TEXT("\""));
			//cmd.append(recFileInfo.GetField(COL_FileInfo_FileName));
			//cmd.append(TEXT("\" "));
		}
		else
		{
			ASSERT(false);
			return;
		}
	}

	fRarLst.flush();
	fRarLst.close();

	tstring cmd = TEXT("a \"");
	cmd.append(pckFileName);
	cmd.append(TEXT("\" @"));
	cmd.append(fnRarLst);

	TTRACE(TEXT("MakePackage Shell : %s\n"), cmd.c_str());

	::ShellExecute(NULL, NULL/*TEXT("open")*/, TEXT("C:\\Program Files\\WinRAR\\WinRAR.exe"), cmd.c_str(), CDwarfOptions::GetInstance().DirCodecFile.c_str(), 0);
}