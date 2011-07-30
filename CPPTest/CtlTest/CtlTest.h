// CtlTest.h : main header file for the CTLTEST application
//

#if !defined(AFX_CTLTEST_H__29725007_98D7_4502_BDB6_E295313AF052__INCLUDED_)
#define AFX_CTLTEST_H__29725007_98D7_4502_BDB6_E295313AF052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCtlTestApp:
// See CtlTest.cpp for the implementation of this class
//

class CCtlTestApp : public CWinApp
{
public:
	CCtlTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtlTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCtlTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTLTEST_H__29725007_98D7_4502_BDB6_E295313AF052__INCLUDED_)
