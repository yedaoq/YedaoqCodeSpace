
// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "Dwarves.h"

#include "ChildFrm.h"
#include "DwarfView.h"
#include "SideWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式

	cs.style &= ~(LONG)FWS_ADDTOTITLE;

	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序
void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	if(bActivate && pActivateWnd && CGlobalData::GetSideWnd())
	{
		CChildFrame* pChildFrame= static_cast<CChildFrame*>(pActivateWnd);
		CDwarfView*  pView		= static_cast<CDwarfView*>(pChildFrame->GetActiveView());
		int			 viewID		= -1;
		if(pView && (viewID = pView->GetViewID()) > 0)
		{
			CGlobalData::GetSideWnd()->ShowRelatedTabsForView(viewID);
		}
	}

	__super::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}