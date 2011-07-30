// TestSendInput.h : main header file for the TESTSENDINPUT application
//

#if !defined(AFX_TESTSENDINPUT_H__E136204B_CBEE_412E_8BDC_C4D6978F984C__INCLUDED_)
#define AFX_TESTSENDINPUT_H__E136204B_CBEE_412E_8BDC_C4D6978F984C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestSendInputApp:
// See TestSendInput.cpp for the implementation of this class
//

class CTestSendInputApp : public CWinApp
{
public:
	CTestSendInputApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestSendInputApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestSendInputApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSENDINPUT_H__E136204B_CBEE_412E_8BDC_C4D6978F984C__INCLUDED_)
