
// HookInstallerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HookInstaller.h"
#include "HookInstallerDlg.h"
#include "DebugLog.h"
//#include "..\CoCreateInstanceHook\CoCreateInstanceHook.h"
#include <Helper.h>

HHOOK*	WINAPI GetHookHandle();
HMODULE	WINAPI GetHookModule();
LRESULT CALLBACK  GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT	WINAPI   Clear();

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHookInstallerDlg 对话框

CHookInstallerDlg::CHookInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHookInstallerDlg::IDD, pParent)
	, m_HWndHook(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHookInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_HWndHook);
}

BEGIN_MESSAGE_MAP(CHookInstallerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CHookInstallerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHookInstallerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CHookInstallerDlg 消息处理程序

BOOL CHookInstallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHookInstallerDlg::OnPaint()
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
HCURSOR CHookInstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHookInstallerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData();
	
	DWORD		idThread = GetWindowThreadProcessId((HWND)m_HWndHook, 0);
	HINSTANCE	hInst	 = GetHookModule();
	HHOOK		hHook	 = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, GetHookModule(), idThread);  

	*GetHookHandle() = hHook;

	if (*GetHookHandle()) 
	{
		//PostThreadMessage(idThread, WM_ENABLEAPIHOOK, 0, 0);
	} 
	else 
	{
		return;
	}
	//WTRACE("WM_ENABLEAPIHOOK");
	return;

}

void CHookInstallerDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
