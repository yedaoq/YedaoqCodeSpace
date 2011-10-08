#include "StdAfx.h"
#include "DBTableViewer4GridCtrl.h"
#include "GridCtrl.h"

using namespace NSDBModule;

CGridColumn	CDBTableViewer4GridCtrl::GetGridColumnFromDBColInfo(CDBColumnViewInfo* info)
{
	CCellTemplate tpl = CCellTemplate(colInfo->GetTextFormat(), colInfo->GetEditStyle());
	return CGridColumn(info->GetTitle(), info->GetEditStyle(), tpl);
}

int	CDBTableViewer4GridCtrl::Initialize()
{
	if(HeadRowCount == -1) HeadRowCount = 1;
	Grid_->SetRowCount(HeadRowCount);
	std::auto_ptr<ColumnEnumerator> pEnumCol = ColumnInfos_.Enum();
	if(!pEnumCol)
	{
		_ASSERT(false);
		return -1;
	}
	
	while(pEnumCol->MoveNext())
	{
		for(int i = Grid_->GetColumnCount() - 1; i <= pEnumCol->Current().IdxView; ++i)
		{
			Grid_->InsertColumn(TEXT(""));
		}

		Grid_->DeleteColumn(pEnumCol->Current().IdxView);

		CGridColumn col = GetGridColumnFromDBColInfo(pEnumCol->Current().ViewInfo);
		Grid_->InsertColumn(col,pEnumCol->Current().IdxView);
	}
}

int	CDBTableViewer4GridCtrl::Fill(const IEnumerator<IDBRecord>& recs)
{
	while(recs.MoveNext())
	{
		NewRecordAt(-1, recs.Current());
	}
}

// record enumerate
int	CDBTableViewer4GridCtrl::GetRecordCount()
{
	return Grid_->GetRowCount() - HeadRowCount;
}

IDBTableViewer::RecordEnumerator* CDBTableViewer4GridCtrl::EnumRecord()
{

}

// selection info
int CDBTableViewer4GridCtrl::GetCurRecord(IDBRecord*)
{

}

DBTableViewColumn CDBTableViewer4GridCtrl::GetCurColumn()
{
	CCellRange range = Grid_->GetSelectedCellRange();
	int if(!range.IsValid())
	{

	}
}

// single record r/w
int CDBTableViewer4GridCtrl::GetRecordAt(int row, IDBRecord*)
{

}

int CDBTableViewer4GridCtrl::DelRecordAt(int row, IDBRecord*)
{

}

int CDBTableViewer4GridCtrl::SetRecordAt(int row, const IDBRecord&)
{

}

int CDBTableViewer4GridCtrl::NewRecordAt(int row, const IDBRecord&)
{

}

// column info
IDBTableViewer::ColumnEnumerator* CDBTableViewer4GridCtrl::EnumColumn()
{

}
