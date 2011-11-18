#pragma once

#include "DwarfDoc.h"
#include <afxwin2.inl>
#include <afxwin.h>
#include "SideWnd.h"
#include "Dwarves.h"
#include "DwarfSideTab.h"
#include "DwarfView.h"
#include "MainFrm.h"

class CSideWnd;
class CDwarfDoc;
class CDwarfSideTab;
class CDwarfView;
class CDwarvesApp;
class CMainFrame;
class CChildFrame;

class CGlobalData
{
public:
	//获取指定的窗口
	static CSideWnd*		GetSideWnd()			{ return m_SideWnd; }
	static void				SetSideWnd(CSideWnd* w)	{ m_SideWnd = w; }

	//获取当前信息
	static CDwarfDoc*		GetCurrentDoc()			{ return &m_Doc; }
	static CDwarfView*		GetCurrentView()		{ return static_cast<CDwarfView*>(static_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveView()); }
	static CDwarfSideTab*	GetCurrentSideTab()		{ return static_cast<CDwarfSideTab*>(m_SideWnd->GetActiveWindow()); }

	//获取指定信息
	static CDwarfView*		GetViewByID(int id)		{ return static_cast<CDwarvesApp*>(AfxGetApp())->GetExistedViewByID(id); }
	static CDwarfSideTab*	GetSideTabByID(int id)	{ return GetSideWnd()->GetDwarfSideTab(id); }

protected:
	static CDwarfDoc		m_Doc;
	static CSideWnd*		m_SideWnd;
};
