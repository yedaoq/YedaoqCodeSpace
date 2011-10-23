#include "StdAfx.h"
#include "DwarfViewInfoDBTblBase.h"
#include <Module\Schema\DBTableSchema.h>
#include <ITextFormater.h>

using namespace NSDBModule;

//CDwarfViewInfoDBTblBase::CDwarfViewInfoDBTblBase(void)
//{
//}

CDwarfViewInfoDBTblBase::~CDwarfViewInfoDBTblBase(void)
{
}

int CDwarfViewInfoDBTblBase::Initialize()
{
	InitializeViewColumns();
	InitializeOperations();
	InitializeReleatedViews();	
}

int CDwarfViewInfoDBTblBase::InitializeReleatedViews()
{

}

int CDwarfViewInfoDBTblBase::InitializeViewColumns()
{
	CDBTableSchema& tblSchema = DBModule.Tables()[ViewID]->GetSchema();
	std::auto_ptr<DBColumnEnumerator> pEnumCol(tblSchema.EnumColumn());

	while(pEnumCol->MoveNext())
	{

	}

}

int CDwarfViewInfoDBTblBase::InitializeOperations()
{

}

CDBColumnViewInfo CDwarfViewInfoDBTblBase::GenerateColumnViewFromSchema(DBColumnSchema& col)
{
	CDBColumnViewInfo view;
	view.SetTitle(col.Name);

	if(col.Type == EnumCppDataType::CppBool)
	{
		view.SetEditStyle(&CEditStyleBool::GetInstance());
		view.SetTextFormat(&CTextFormatSwitcherNone::GetInstance());
	}
	else if(col.RelyTblID >= 0 && col.RelyColID >= 0)
	{
		view.SetEditStyle(&CDBEnumeratorProvider.GetInstance()[col.RelyTblID].GetFieldEnumerator(col.RelyColID));
		if(col.VisiColID >= 0 && col.VisiColID != col.RelyColID)
		{
			view.SetTextFormat();
		}
		else
		{
			view.SetTextFormat(&CTextFormatSwitcherNone::GetInstance());
		}
	}
}