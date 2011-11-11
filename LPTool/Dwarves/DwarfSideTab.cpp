#include "StdAfx.h"
#include "DwarfSideTab.h"
#include "DwarfViewInfo.h"

CDwarfSideTab::CDwarfSideTab(void)
	: FlowLayoutMain(EnumLayoutDirection::Vertical), GridViewer(&Grid, 1), View(0), ValidityCounter(-1)
{
}

CDwarfSideTab::~CDwarfSideTab(void)
{
}

BEGIN_MESSAGE_MAP(CDwarfSideTab, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CDwarfSideTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rect = {0, 0, 10, 10};
	Grid.Create(rect, this, EIDC_GRID);

	FlowLayoutMain.AddCtrl(Grid.GetSafeHwnd(), 
		ResizeInfo::FillInfo, 
		ResizeInfo::FillInfo,
		AnchorInfo(EnumAnchorMode::Front, 0),
		AnchorInfo(EnumAnchorMode::Front, 0));

	return 0;
}

void CDwarfSideTab::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
	FlowLayoutMain.Layout(NSYedaoqLayout::LayoutPoint(0,0), NSYedaoqLayout::LayoutSize(cx, cy));
}

int CDwarfSideTab::Initialize(IDwarfViewInfo* pView)
{
	if(!pView) return -1;
	View = pView;

	GridViewer.Initialize(pView->GetViewColumnCollection());

	return 1;
}

bool CDwarfSideTab::IsRelatedToMainView()
{
	return true;
}

int CDwarfSideTab::GetViewID() 
{
	if(View)
	{
		return View->GetViewID();
	}

	return -1;
}

int CDwarfSideTab::GetValidityCounter()
{
	return ValidityCounter;
}

int CDwarfSideTab::SetValidityCounter(int val)
{
	ValidityCounter = val;
}

int CDwarfSideTab::ContentUpdate(int mainViewID, DwarfViewOperationContext* pCtx)
{
	IDwarfViewInfo* pMainView = CDwarfViewProvider::GetInstance()[mainViewID];
	std::auto_ptr<IEnumerator<IDBRecord>> pEnumRec(View->EnumRecordAsRelatedView(pMainView, pCtx));
	if(pEnumRec.get())
	{
		GridViewer.Clear();
		GridViewer.Fill(*pEnumRec);
	}

	return 1;
}