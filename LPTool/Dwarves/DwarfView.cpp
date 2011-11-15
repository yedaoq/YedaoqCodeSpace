
// DwarfView.cpp : CDwarfView 类的实现
//

#include "stdafx.h"
#include "Dwarves.h"
#include "DwarfDoc.h"
#include "DwarfView.h"
#include "CommandIDAlloter.h"
#include "ChildFrm.h"
#include <memory>
#include "DwarfViewInfo.h"
#include "DwarfViewProvider.h"
#include "SideWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NSYedaoqLayout;

// CDwarfView

IMPLEMENT_DYNCREATE(CDwarfView, CView)

BEGIN_MESSAGE_MAP(CDwarfView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDwarfView::OnFilePrintPreview)
	ON_NOTIFY(GVN_SELCHANGED, EIDC_GRID, &CDwarfView::OnGridSelChanged)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CONTROL_RANGE(BN_CLICKED, MinViewOpID, MaxViewOpID, &CDwarfView::OnViewOperation)
END_MESSAGE_MAP()

// CDwarfView 构造/析构

CDwarfView::CDwarfView()
	: FlowLayoutMain(EnumLayoutDirection::Vertical), GridViewer(&Grid, 1), ViewID(-1)
{
	// TODO: 在此处添加构造代码
	
}

CDwarfView::~CDwarfView()
{
}

BOOL CDwarfView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDwarfView 绘制

void CDwarfView::OnDraw(CDC* /*pDC*/)
{
	CDwarfDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

int CDwarfView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
		return -1;

	RECT rect = {0, 0, lpcs->cx, lpcs->cy};
	Grid.Create(rect, this, EIDC_GRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);

	CreateButton(BtnSearch, EIDC_BTNSEARCH, this, TEXT("查找"));

	CFlowLayout* pFlow = FlowLayoutMain.AddFlow(EnumLayoutDirection::Horizon);
	pFlow->AddCtrl(BtnSearch.GetSafeHwnd());

	FlowLayoutMain.AddCtrl(Grid.GetSafeHwnd(), ResizeInfo::FillInfo, ResizeInfo::FillInfo);

	return 0;
}

void CDwarfView::OnSize(UINT nType, int cx, int cy)
{
	FlowLayoutMain.Layout(LayoutPoint(3,3), LayoutSize(cx - 6, cy - 6));
}

//void CDwarfView::OnInitialUpdate()
//{
//	__super::OnInitialUpdate();
//	ResetTitle();
//}

void CDwarfView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	__super::OnUpdate(pSender, lHint, pHint);
}

// CDwarfView 打印


void CDwarfView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CDwarfView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDwarfView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDwarfView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDwarfView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDwarfView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

void CDwarfView::OnViewOperation(UINT id)
{
	int opID = id - MinViewOpID;
	
}

void CDwarfView::OnGridSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CDBModule&		module = GetDocument()->GetDBModule();
	CDBRecordAuto	rec;
	GridViewer.GetCurRecord(&rec); 
}

// CDwarfView 诊断

#ifdef _DEBUG
void CDwarfView::AssertValid() const
{
	CView::AssertValid();
}

void CDwarfView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDwarfDoc* CDwarfView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDwarfDoc)));
	return (CDwarfDoc*)m_pDocument;
}
#endif //_DEBUG

void CDwarfView::SetViewID(int id)
{ 
	if(id < 0)
	{
		_ASSERT(false);
		return;
	}

	ViewID = id; 

	ResetTitle();
	Initialize();
	ShowRecords();

	CChildFrame* pChildFrame = static_cast<CChildFrame*>(GetParentFrame());
	pChildFrame->OnMDIActivate(TRUE, pChildFrame, NULL);
}

void CDwarfView::ResetTitle()
{
	CChildFrame* pChildFrm = (CChildFrame*)GetParent();

	if(ViewID >= 0)
	{
		pChildFrm->SetWindowText(GetDocument()->GetDBModule()->Tables()[ViewID]->GetName().c_str());
	}
	else
	{
		pChildFrm->SetWindowText(TEXT("未知"));
	}
}

int CDwarfView::Initialize()
{	
	IDwarfViewInfo* pView = CDwarfViewProvider::GetInstance()[ViewID];

	if(!pView)
	{
		_ASSERT(false);
		return 0;
	}

	pView->Initialize();

	CFlowLayout* pFlowOps = static_cast<CFlowLayout*>(FlowLayoutMain[0]);
	pFlowOps->Clear();
	pFlowOps->AddCtrl(BtnSearch.GetSafeHwnd());

	for(OpBtnCollection::iterator iter = BtnOps.begin(); iter != BtnOps.end(); ++iter)
	{
		delete *iter;
	}
	BtnOps.clear();

	int DlgItemID = EIDC_UNUSED;
	std::auto_ptr<IEnumerator<DwarfViewOperationItem>> pEnumOp(pView->EnumOperation());
	if(pEnumOp.get())
	{	
		while(pEnumOp->MoveNext())
		{
			BtnOps.push_back(new CButton());
			CreateButton(**BtnOps.rbegin(), EIDC_UNUSED + pEnumOp->Current().ID, this, pEnumOp->Current().Name.c_str());
			pFlowOps->AddCtrl((*BtnOps.rbegin())->GetSafeHwnd());
		}
	}
	
	GridViewer.Clear();
	GridViewer.Initialize(pView->GetViewColumnCollection());

	RECT rect;
	this->GetClientRect(&rect);
	OnSize(0, rect.right, rect.bottom);

	return 0;
}

int CDwarfView::ShowRecords()
{
	IDwarfViewInfo* pView = CDwarfViewProvider::GetInstance()[ViewID];

	if(!pView)
	{
		_ASSERT(false);
		return 0;
	}

	std::auto_ptr<IEnumerator<IDBRecord>> pEnumRec(pView->EnumRecord());
	GridViewer.Fill(*pEnumRec);
	
	Grid.AutoSizeColumns();
}


// CDwarfView 消息处理程序

void CDwarfView::CreateButton(CButton& btn, UINT id, CWnd* pParent, LPCTSTR lpTitle, UINT width, UINT height, DWORD dwStyle, CFont* pFont)
{
	static CFont* pDefaultFont = 0;

	if(!pDefaultFont)
	{
		pDefaultFont = new CFont;
		pDefaultFont->CreateFont(16,		//   nHeight
			0,								//   nWidth
			0,								//   nEscapement
			0,								//   nOrientation
			0,								//   nWeight
			FALSE,							//   bItalic
			FALSE,							//   bUnderline
			0,								//   cStrikeOut
			ANSI_CHARSET,					//   nCharSet
			OUT_DEFAULT_PRECIS,				//   nOutPrecision
			CLIP_DEFAULT_PRECIS,			//   nClipPrecision
			DEFAULT_QUALITY,				//   nQuality
			DEFAULT_PITCH   |   FF_SWISS,	//   nPitchAndFamily
			_T( "Arial "));					//   lpszFac 
	}

	RECT rect = {0, 0, width, height};
	btn.Create(lpTitle, dwStyle, rect, pParent, id);
	if(!pFont) pFont = pDefaultFont;
	btn.SetFont(pFont);
	btn.ShowWindow(SW_SHOW);
}