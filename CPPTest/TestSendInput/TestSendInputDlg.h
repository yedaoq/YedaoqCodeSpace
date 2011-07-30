// TestSendInputDlg.h : header file
//

#if !defined(AFX_TESTSENDINPUTDLG_H__5D387E40_3A5A_432F_B242_DB072B597EE6__INCLUDED_)
#define AFX_TESTSENDINPUTDLG_H__5D387E40_3A5A_432F_B242_DB072B597EE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Log.h"

/////////////////////////////////////////////////////////////////////////////
// CTestSendInputDlg dialog

class CTestSendInputDlg : public CDialog
{
// Construction
public:
	CTestSendInputDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestSendInputDlg)
	enum { IDD = IDD_TESTSENDINPUT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestSendInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CLog * m_pLog;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestSendInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSENDINPUTDLG_H__5D387E40_3A5A_432F_B242_DB072B597EE6__INCLUDED_)
