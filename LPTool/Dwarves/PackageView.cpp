
// PackageView.cpp : CPackageView 类的实现
//

#include "stdafx.h"
#include "Dwarves.h"

#include "PackageDoc.h"
#include "PackageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPackageView

IMPLEMENT_DYNCREATE(CPackageView, CView)

BEGIN_MESSAGE_MAP(CPackageView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPackageView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CPackageView 构造/析构

CPackageView::CPackageView()
{
	// TODO: 在此处添加构造代码

}

CPackageView::~CPackageView()
{
}

BOOL CPackageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPackageView 绘制

void CPackageView::OnDraw(CDC* /*pDC*/)
{
	CPackageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPackageView 打印


void CPackageView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CPackageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPackageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPackageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CPackageView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPackageView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CPackageView 诊断

#ifdef _DEBUG
void CPackageView::AssertValid() const
{
	CView::AssertValid();
}

void CPackageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPackageDoc* CPackageView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPackageDoc)));
	return (CPackageDoc*)m_pDocument;
}
#endif //_DEBUG


// CPackageView 消息处理程序
