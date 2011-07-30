// CtlTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CtlTest.h"
#include "CtlTestDlg.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtlTestDlg dialog

CCtlTestDlg::CCtlTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCtlTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCtlTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCtlTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCtlTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCtlTestDlg, CDialog)
	//{{AFX_MSG_MAP(CCtlTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtlTestDlg message handlers

BOOL CCtlTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCtlTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCtlTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCtlTestDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	
}

void CCtlTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
	HWND hWnd = ::GetDlgItem(this->m_hWnd, IDC_CHECK1);
	::PostMessage(hWnd, BM_CLICK, 0, 0);
	//::PostMessage(hWnd, WM_LBUTTONDBLCLK, 1, 1);
	
	//SendMessage(BM_CLICK, 0, (long)hWnd);
}
