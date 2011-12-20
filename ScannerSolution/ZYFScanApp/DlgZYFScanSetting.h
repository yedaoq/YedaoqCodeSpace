#pragma once

#include "GridCtrl.h"
#include "GridCell.h"
#include "CZYFSCanOptions.h"


// CDlgZYFScanSetting dialog

class CDlgZYFScanSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgZYFScanSetting)

public:
	CDlgZYFScanSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgZYFScanSetting();

// Dialog Data
	enum { IDD = IDD_DLGSCANSETTING };

public:
	void Initialize();
	void Load(const CZYFScanOptions& options );


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	
public:
	CGridCtrl	m_GridBatch;
	CGridCtrl	m_GridItem;
	int			m_GridBatchRow;

	CZYFScanOptions	m_Options;
	afx_msg void OnBnClickedButaddbatch();
	afx_msg void OnBnClickedBtndelbatch();
	afx_msg void OnBnClickedBtnadditem();
	afx_msg void OnBnClickedBtndelitem();
	afx_msg void OnBnClickedOk();
	afx_msg void OnGridBatchSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult);
};
