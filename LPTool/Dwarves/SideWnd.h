
#pragma once
#include <map>
#include "DwarfViewInfo.h"

interface IDwarfViewInfo;
class	  CDwarfSideTab;

class CSideWnd : public CDockablePane
{
// 构造
public:
	typedef std::map<int, CDwarfSideTab*> DwarfSideTabMap;

	CSideWnd();

	void ShowRelatedTabsForView(int viewID);
	void ClearTabs();

	CDwarfSideTab*				GetDwarfSideTab(int view);

// 属性
protected:
	CFont						m_Font;

	CMFCTabCtrl					m_wndTabs;
	int							m_ValidityCounter;

	DwarfViewOperationContext	m_Context;
	int							m_MainViewID;

	DwarfSideTabMap				m_SideTabCache;

protected:
	void IncreaseValidityCounter();

	void AdjustHorzScroll(CListBox& wndListBox);

// 实现
public:
	virtual ~CSideWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnTabActivate(WPARAM wParam, LPARAM lParam);

	afx_msg void OnMainViewActivated(int mainView);
	afx_msg void OnMainViewContextChanged(const DwarfViewOperationContext* ctx);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COutputList 窗口

class COutputList : public CListBox
{
	// 构造
public:
	COutputList();

	// 实现
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};
