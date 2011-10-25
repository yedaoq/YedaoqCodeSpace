#include "StdAfx.h"
#include "DwarfViewInfoDBTblBase.h"
#include <Module\Schema\DBTableSchema.h>
#include <ITextFormater.h>
#include "DBTableViewer4GridCtrl.h"
#include <Module\EnumCppDataType.h>
#include <EditStyle.h>
#include "DBEnumeratorProvider.h"
#include <Util\DBEnumeratorSuit.h>
#include "DBMapFormatProvider.h"
#include "DBOptionalEditStyleProvider.h"
#include "DwarfViewProvider.h"

using namespace NSDBModule;

bool CDwarfViewInfoDBTblBase::IsDBTableRelated(int tblCur, int tblTar, int* colCur, int* colTar)
{
	if(tblCur < 0 || tblCur >= g_DBModule.Tables().Count() || tblTar < 0 || tblTar >= g_DBModule.Tables().Count())
	{
		return false;
	}

	std::auto_ptr<DBColumnEnumerator> pEnumCol(g_DBModule.Tables()[tblCur]->EnumColumn());
	while(pEnumCol->MoveNext())
	{
		if(pEnumCol->Current().RelyTblID == tblTar)
		{
			if(colCur) *colCur = pEnumCol->Current().Index;
			if(colTar) *colTar = pEnumCol->Current().RelyColID;
			return true;
		}
	}

	return false;
}

CDwarfViewInfoDBTblBase::~CDwarfViewInfoDBTblBase(void)
{
}

int CDwarfViewInfoDBTblBase::Initialize()
{
	InitializeColumnViewInfo();
	InitializeOperations();
	InitializeReleatedViews();	

	return 1;
}

int CDwarfViewInfoDBTblBase::InitializeReleatedViews()
{
	std::auto_ptr<DBTableEnumerator> pEnumTbl(this->DBModule->Tables().Enum());
	
	for(int idx = 0; idx < DBModule->Tables().Count(); ++idx)
	{
		if(IsDBTableRelated(idx, GetViewID(), 0, 0))
		{
			this->ReleatedViews.push_back(
				CDwarfViewProvider::GetInstance()[idx]);
		}
	}

	std::auto_ptr<DBColumnEnumerator> pEnumCol(this->DBModule->Tables()[this->GetViewID()]->EnumColumn());
	while(pEnumCol->MoveNext())
	{
		if(pEnumCol->Current().RelyTblID != DBColumnSchema::InvalidRelyTableID)
		{
			this->ReleatedViews.push_back(
				CDwarfViewProvider::GetInstance()[pEnumCol->Current().RelyTblID]);
		}
	}

	return 1;
}

int CDwarfViewInfoDBTblBase::InitializeOperations()
{
	this->Operations.Append(TEXT("修改"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTblBase::OnRecordModify));
	this->Operations.Append(TEXT("添加"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTblBase::OnRecordInsert));
	this->Operations.Append(TEXT("删除"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTblBase::OnRecordDelete));
	
	return 1;
}

int CDwarfViewInfoDBTblBase::InitializeViewColumns()
{
	InitializeViewColumns();

	for (DBColumnViewInfoCollection::iterator iter = ColumnViewInfos.begin(); iter != ColumnViewInfos.end(); ++iter)
	{
		ViewColumns.Append(&(*iter));
	}
	return 1;
}

int CDwarfViewInfoDBTblBase::InitializeColumnViewInfo()
{
	CDBTableSchema& tblSchema = DBModule->Tables()[ViewID]->GetSchema();
	std::auto_ptr<DBColumnEnumerator> pEnumCol(tblSchema.EnumColumn());

	while(pEnumCol->MoveNext())
	{
		ColumnViewInfos.push_back(GenerateColumnViewFromSchema(pEnumCol->Current()));
	}
	return 1;
}

CDBColumnViewInfo CDwarfViewInfoDBTblBase::GenerateColumnViewFromSchema(const DBColumnSchema& col)
{
	CDBColumnViewInfo view;
	view.SetTitle(col.Name);
	view.SetDefaultWidth(-1);

	if(col.Type == EnumCppDataType::CppBool)
	{
		view.SetEditStyle(&CEditStyleBool::GetInstance());
		view.SetTextFormat(&CTextFormatSwitcherNone::GetInstance());
	}
	else if(col.RelyTblID >= 0 && col.RelyColID >= 0)
	{
		view.SetEditStyle(&CDBOptionalEditStyleProvider::GetInstance().Get(col.RelyTblID, col.RelyColID));
		if(col.VisiColID >= 0 && col.VisiColID != col.RelyColID)
		{
			view.SetTextFormat(&CDBMapFormatProvider::GetInstance().Get(col.RelyTblID, col.RelyColID, col.VisiColID));
		}
		else
		{
			view.SetTextFormat(&CTextFormatSwitcherNone::GetInstance());
		}
	}

	return view;
}

IEnumerator<IDBRecord>*	CDwarfViewInfoDBTblBase::EnumRecordAsRelatedView(IDwarfViewInfo* pView, DwarfViewOperationContext* pCtx)
{
	// 1. 判断从属视图是否依赖了主视图，若是，则根据该依赖关系过滤
	// 2. 判断主视图是否依赖了从属视图，若是，则根据该依赖关系过滤

	if(!pView || !pCtx->pSelectedRecords || !pCtx->pSelectedRecords->MoveNext())
	{
		_ASSERT(false);
		return 0;
	}

	int iRelatedTblID = DBColumnSchema::InvalidRelyTableID;
	int iRelatedColIDOfCurrentTbl = -1;
	int iRelatedColIDOfMainTbl = -1;

	if(IsDBTableRelated(GetViewID(), pView->GetViewID(), &iRelatedColIDOfCurrentTbl, &iRelatedColIDOfMainTbl))
	{
		iRelatedTblID = pView->GetViewID();
	}
	else if(IsDBTableRelated(pView->GetViewID(), GetViewID(), &iRelatedColIDOfMainTbl, &iRelatedColIDOfCurrentTbl))
	{
		iRelatedTblID = pView->GetViewID();
	}

	if(iRelatedTblID == DBColumnSchema::InvalidRelyTableID)
	{
		return 0;
	}

	CDBRecordBase rec = DBModule->Tables()[GetViewID()]->RecordTemplate();
	rec.SetField(iRelatedColIDOfCurrentTbl, pCtx->pSelectedRecords->Current().GetField(iRelatedColIDOfMainTbl));
	CDBRecordComparison cmp(1, iRelatedColIDOfCurrentTbl);

	return DBModule->Tables()[GetViewID()]->FindAll(rec, cmp);
}

IEnumerator<IDBRecord>*	CDwarfViewInfoDBTblBase::EnumRecord()
{
	return DBModule->Tables()[GetViewID()]->EnumRecord();
}

void CDwarfViewInfoDBTblBase::OnRecordModify(DwarfViewOperationContext* pCtx)
{

}

void CDwarfViewInfoDBTblBase::OnRecordDelete(DwarfViewOperationContext* pCtx)
{

}

void CDwarfViewInfoDBTblBase::OnRecordInsert(DwarfViewOperationContext* pCtx)
{

}