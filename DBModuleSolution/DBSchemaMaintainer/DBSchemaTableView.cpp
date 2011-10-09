
// DBSchemaTableView.cpp : CDBSchemaTableView 类的实现
//

#include "stdafx.h"
#include "DBSchemaMaintainer.h"

#include "DBSchemaMaintainerDoc.h"
#include "DBSchemaTableView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NSYedaoqLayout;

// CDBSchemaTableView

IMPLEMENT_DYNCREATE(CDBSchemaTableView, CView)

BEGIN_MESSAGE_MAP(CDBSchemaTableView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDBSchemaTableView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDBSchemaTableView 构造/析构

CDBSchemaTableView::CDBSchemaTableView()
	: Layout_(EnumLayoutDirection::Vertical)
{
	// TODO: 在此处添加构造代码

}

CDBSchemaTableView::~CDBSchemaTableView()
{
}

BOOL CDBSchemaTableView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	//cs.style ^= WS_BORDER;

	return CView::PreCreateWindow(cs);
}

int CDBSchemaTableView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
		return -1;

	RECT rect = {0, 0, lpcs->cx, lpcs->cy};
	Grid_.Create(rect, this, 1, WS_CHILD | WS_TABSTOP | WS_VISIBLE);

	RECT rect1 = { 0, 0, 150, 28 };
	CmbTables_.Create(CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP, rect1, this, 2);

	CmbTables_.ShowWindow(SW_SHOW);

	CFlowLayout* pFlow = Layout_.AddFlow(EnumLayoutDirection::Horizon);
	pFlow->AddCtrl(CmbTables_.GetSafeHwnd());

	Layout_.AddCtrl(
		Grid_.GetSafeHwnd(), 
		ResizeInfo::FillInfo, 
		ResizeInfo::FillInfo);

	Grid_.SetColumnCount(2);
	Grid_.SetRowCount(1);

	return S_OK;
}

// CDBSchemaTableView 绘制

void CDBSchemaTableView::OnDraw(CDC* /*pDC*/)
{
	CDBSchemaMaintainerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CDBSchemaTableView 打印


void CDBSchemaTableView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CDBSchemaTableView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDBSchemaTableView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDBSchemaTableView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDBSchemaTableView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDBSchemaTableView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

afx_msg void CDBSchemaTableView::OnSize(UINT nType, int cx, int cy)
{
	Layout_.Layout(LayoutPoint(0, 0), LayoutSize(cx, cy));
}

// CDBSchemaTableView 诊断

#ifdef _DEBUG
void CDBSchemaTableView::AssertValid() const
{
	CView::AssertValid();
}

void CDBSchemaTableView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDBSchemaMaintainerDoc* CDBSchemaTableView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDBSchemaMaintainerDoc)));
	return (CDBSchemaMaintainerDoc*)m_pDocument;
}
#endif //_DEBUG


// CDBSchemaTableView 消息处理程序
