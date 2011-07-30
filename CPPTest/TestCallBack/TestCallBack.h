// TestCallBack.h : main header file for the TESTCALLBACK DLL
//

#if !defined(AFX_TESTCALLBACK_H__0059EA00_14B3_4D95_AF7E_63B40F2B2F8C__INCLUDED_)
#define AFX_TESTCALLBACK_H__0059EA00_14B3_4D95_AF7E_63B40F2B2F8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestCallBackApp
// See TestCallBack.cpp for the implementation of this class
//

class CTestCallBackApp : public CWinApp
{
public:
	CTestCallBackApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestCallBackApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTestCallBackApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCALLBACK_H__0059EA00_14B3_4D95_AF7E_63B40F2B2F8C__INCLUDED_)
