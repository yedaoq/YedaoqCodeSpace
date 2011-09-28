
// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "..\YedaoqLayout\ZoomBlankLayout.h"
#include "..\YedaoqLayout\CtrlLayout.h"
#include "Dialog1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestDlg 对话框

using namespace NSYedaoqLayout;


CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTestDlg::IDD, pParent), m_Layout(EnumLayoutDirection::Vertical)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CmbTest);
	DDX_Control(pDX, IDOK, m_BtnOK);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_LIST1, m_LstTest);
	DDX_Control(pDX, IDC_TREE1, m_TreTest);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CFlowLayout* pFlowLayout = m_Layout.AddFlow(
		EnumLayoutDirection::Horizon);
	
	CCtrlLayout* pCtrlLayout = pFlowLayout->AddCtrl(m_CmbTest.GetSafeHwnd(),
		ResizeInfo::FixedInfo,
		ResizeInfo::FixedInfo,
		AnchorInfo::AnchorFront,
		AnchorInfo::AnchorCenter);

	pFlowLayout->AddZoomBlank();
	pCtrlLayout = pFlowLayout->AddCtrl(m_BtnOK.GetSafeHwnd());
	pCtrlLayout = pFlowLayout->AddCtrl(m_BtnCancel.GetSafeHwnd());

	pFlowLayout = m_Layout.AddFlow(
		EnumLayoutDirection::Horizon, 
		ResizeInfo(EnumResizeMode::Zoomed, 100));

	pCtrlLayout = pFlowLayout->AddCtrl(m_TreTest.GetSafeHwnd(),
		ResizeInfo(EnumResizeMode::Zoomed, 30),
		ResizeInfo(EnumResizeMode::Zoomed, 100));

	pCtrlLayout = pFlowLayout->AddCtrl(m_LstTest.GetSafeHwnd(),
		ResizeInfo(EnumResizeMode::Zoomed, 70),
		ResizeInfo(EnumResizeMode::Zoomed, 100));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_CmbTest.GetSafeHwnd())
	{
		m_Layout.Layout(LayoutPoint(3, 3), LayoutSize(cx - 6, cy - 6));
		this->Invalidate(FALSE);
	}
}

void CTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog1 dlg;
	dlg.DoModal();
	//OnOK();
}
