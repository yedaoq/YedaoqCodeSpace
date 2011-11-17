#pragma once

class CGlobalData
{
public:
	static CSideWnd*		GetCurrentSideWnd()		{ return m_SideWnd; }
	static CDwarfDoc*		GetCurrentDoc()			{ return &m_Doc; }
	static CDwarfView*		GetCurrentView()		{ return static_cast<CDwarfView*>(static_cast<CMainFrame*>(AfxGetMainWnd()).GetActiveView()); }
	static CDwarfSideTab*	GetCurrentSideTab()		{ return static_cast<CDwarfSideTab*>(m_SideWnd->GetActiveWindow()); }

	static 

protected:
	static CDwarfDoc	m_Doc;
	static CSideWnd*	m_SideWnd;
};
