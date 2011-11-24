
#pragma once
#include <map>
#include "DwarfViewInfo.h"

interface IDwarfViewInfo;
class	  CDwarfSideTab;

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


class CSideWnd : public CDockablePane
{
// 构造
public:
	typedef std::map<int, CDwarfSideTab*> DwarfSideTabMap;

	CSideWnd();

	void ShowRelatedTabsForView(int viewID);
	void RefreshSideView();
	void ClearTabs();

	CDwarfSideTab*				GetDwarfSideTab(int view, bool autoCreate = true);

// 属性
protected:
	CFont						m_Font;

	CMFCTabCtrl					m_wndTabs;
	int							m_ValidityCounter;

	DwarfViewOperationContext	m_Context;

	DwarfSideTabMap				m_SideTabCache;

	COutputList					m_wndOutputBuild;

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
