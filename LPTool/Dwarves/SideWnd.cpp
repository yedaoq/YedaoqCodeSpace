
#include "stdafx.h"

#include "SideWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "SideTab.h"
#include "DwarfViewInfo.h"
#include "DwarfViewProvider.h"
#include "DwarfSideTab.h"
#include <Helper.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSideBar

CSideWnd::CSideWnd()
{
	m_Context.MainViewID = CDwarfViewProvider::InvalidViewID;
	m_Context.SideViewID = CDwarfViewProvider::InvalidViewID;
}

CSideWnd::~CSideWnd()
{
}

BEGIN_MESSAGE_MAP(CSideWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, &CSideWnd::OnTabActivate)
END_MESSAGE_MAP()

int CSideWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateStockObject(DEFAULT_GUI_FONT);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("未能创建输出选项卡窗口\n");
		return -1;      // 未能创建
	}
	m_wndTabs.AutoDestroyWindow(FALSE);

	// 创建日志窗格:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndLogTab.Create(dwStyle, rectDummy, &m_wndTabs, 2))
	{
		TRACE0("未能创建输出窗口\n");
		return -1;      // 未能创建
	}

	// 将日志窗口附加到选项卡:
	CGlobalData::SetLogTab(&m_wndLogTab);
	//m_wndLogTab.BeginMonit(&CDBModuleLP::GetInstance());
	m_wndLogTab.SetFont(&m_Font);
	m_wndTabs.AddTab(&m_wndLogTab, TEXT("日志"), (UINT)0);

	return 0;
}

void CSideWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT CSideWnd::OnTabActivate(WPARAM wParam, LPARAM lParam)
{
	int idTabActive = m_wndTabs.GetActiveTab();
	ASSERT(idTabActive == (int)wParam);

	CWnd* pWndActive = m_wndTabs.GetActiveWnd();
	if(!pWndActive) return 0;

	ISideTab* pSideTab = dynamic_cast<ISideTab*>(pWndActive);
	
	if(pSideTab->GetValidityCounter() != m_ValidityCounter && m_Context.MainViewID != CDwarfViewProvider::InvalidViewID)
	{
		pSideTab->ContentUpdate(&m_Context);
		pSideTab->SetValidityCounter(m_ValidityCounter);
	}
}

void CSideWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_Font);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void CSideWnd::IncreaseValidityCounter()
{
	++m_ValidityCounter;
}

void CSideWnd::ShowRelatedTabsForView(int viewID)
{
	ClearTabs();
	m_wndTabs.AddTab(&m_wndLogTab, TEXT("日志"));

	IDwarfViewInfo* pView = CDwarfViewProvider::GetInstance()[viewID];

	if(!pView) return;

	m_Context.MainViewID = viewID;

	std::auto_ptr<IEnumerator<IDwarfViewInfo*>> pEnumView(pView->EnumReleatedView());

	if(!pEnumView.get()) return;

	TTRACE(TEXT("初始化附加视图列表 ================\n"));

	while(pEnumView->MoveNext())
	{
		CDwarfSideTab* tab = GetDwarfSideTab(pEnumView->Current()->GetViewID(), true);

		TTRACE(TEXT("\t%d - 0X%08X\n"), tab->GetViewID(), tab->GetSafeHwnd());

		if(tab && -1 == m_wndTabs.GetTabFromHwnd(tab->GetSafeHwnd()))
		{
			m_wndTabs.AddTab(tab, pEnumView->Current()->ToString().c_str());
		}		
	}
}

void CSideWnd::RefreshSideView()
{
	IncreaseValidityCounter();
	OnTabActivate(m_wndTabs.GetActiveTab(), 0);	
}

int CSideWnd::GetCurrentSideViewID()
{
	CWnd* pWndActive = m_wndTabs.GetActiveWnd();
	if(!pWndActive) return -1;

	ISideTab* pSideTab = dynamic_cast<ISideTab*>(pWndActive);
	if(pSideTab && pSideTab->IsRelatedToMainView())
	{
		return pSideTab->GetViewID();
	}

	return -1;
}

void CSideWnd::ClearTabs()
{
	/*for (int i = m_wndTabs.GetTabsNum() - 1; i >= 0; --i)
	{
		m_wndTabs.DetachTab(AFX_DOCK_METHOD::DM_STANDARD, i, TRUE);
	}*/
	m_wndTabs.RemoveAllTabs();
}

CDwarfSideTab* CSideWnd::GetDwarfSideTab(int view, bool autoCreate)
{
	DwarfSideTabMap::iterator iter = m_SideTabCache.find(view);
	if(m_SideTabCache.end() == iter)
	{
		if(!autoCreate) return 0;

		iter = m_SideTabCache.insert(std::make_pair(view, new CDwarfSideTab())).first;
		iter->second->Initialize(&m_wndTabs, CDwarfViewProvider::GetInstance()[view]);
	}
	return iter->second;
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 消息处理程序

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("复制输出"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("清除输出"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}


