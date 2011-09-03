
// DBSchemaMaintainerDlg.h : header file
//

#pragma once


// CDBSchemaMaintainerDlg dialog
class CDBSchemaMaintainerDlg : public CDialogEx
{
// Construction
public:
	CDBSchemaMaintainerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DBSCHEMAMAINTAINER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
