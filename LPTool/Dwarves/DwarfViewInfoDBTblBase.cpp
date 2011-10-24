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

using namespace NSDBModule;

CDwarfViewInfoDBTblBase::~CDwarfViewInfoDBTblBase(void)
{
}

int CDwarfViewInfoDBTblBase::Initialize()
{
	InitializeColumnViewInfo();
	InitializeOperations();
	InitializeReleatedViews();	
}

int CDwarfViewInfoDBTblBase::InitializeReleatedViews()
{
	

	return 1;
}

int CDwarfViewInfoDBTblBase::InitializeOperations()
{
	this->Operations.Append(TEXT("ÐÞ¸Ä"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTblBase::OnRecordModify));
	this->Operations.Append(TEXT("Ìí¼Ó"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTblBase::OnRecordInsert));
	this->Operations.Append(TEXT("É¾³ý"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTblBase::OnRecordDelete));
	
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

void CDwarfViewInfoDBTblBase::OnRecordModify(DwarfViewOperationContext* pCtx)
{

}

void CDwarfViewInfoDBTblBase::OnRecordDelete(DwarfViewOperationContext* pCtx)
{

}

void CDwarfViewInfoDBTblBase::OnRecordInsert(DwarfViewOperationContext* pCtx)
{

}