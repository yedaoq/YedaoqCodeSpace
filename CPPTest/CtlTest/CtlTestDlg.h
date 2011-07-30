// CtlTestDlg.h : header file
//

#if !defined(AFX_CTLTESTDLG_H__942C2AB3_5EB5_43F3_910B_2C1172F400CC__INCLUDED_)
#define AFX_CTLTESTDLG_H__942C2AB3_5EB5_43F3_910B_2C1172F400CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCtlTestDlg dialog

class CCtlTestDlg : public CDialog
{
// Construction
public:
	CCtlTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCtlTestDlg)
	enum { IDD = IDD_CTLTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtlTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCtlTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCheck1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTLTESTDLG_H__942C2AB3_5EB5_43F3_910B_2C1172F400CC__INCLUDED_)
