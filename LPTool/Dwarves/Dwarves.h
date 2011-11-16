
// Dwarves.h : Dwarves 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "DwarfDoc.h"


// CDwarvesApp:
// 有关此类的实现，请参阅 Dwarves.cpp
//

class CSideWnd;
class CDwarfView;

class CDwarvesApp : public CWinAppEx
{
public:
	CDwarvesApp();


// 重写
public:
	virtual BOOL InitInstance();

	CDwarfView* GetExistedViewByID(int id);
	CDwarfView* NewView(int id);

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	CDwarfDoc	m_Doc;
	CSideWnd*	m_SideWnd;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	
	afx_msg void OnFileOpen();
	afx_msg void OnViewOpen(UINT id);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDwarvesApp theApp;
