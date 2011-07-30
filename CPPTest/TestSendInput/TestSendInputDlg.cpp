// TestSendInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestSendInput.h"
#include "TestSendInputDlg.h"
#include <winable.h>
#include <atlconv.h>
#include "Assist.h"

#pragma comment(lib,"YedaoqCppLib.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void SendAscii(wchar_t data, BOOL shift)
{
  INPUT input[2];
  memset(input, 0, 2 * sizeof(INPUT));
 
  if (shift)
  {
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = VK_SHIFT;
    SendInput(1, input, sizeof(INPUT));
  }

  input[0].type = INPUT_KEYBOARD;
  input[0].ki.wVk = data;
  //input[0].ki.time = GetTickCount();
  //SendInput(1, input, sizeof(INPUT));

  input[1].type = INPUT_KEYBOARD;
  input[1].ki.wVk = data + 1;
  //input[1].ki.time = GetTickCount() + 100;
  //input[1].ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(2, input, sizeof(INPUT));

  if (shift)
  {
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = VK_SHIFT;
    input[0].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, input, sizeof(INPUT));  
  }
}

void SendUnicode(wchar_t data)
{
  INPUT input[2];
  memset(input, 0, 2 * sizeof(INPUT));
 
  input[0].type = INPUT_KEYBOARD;
  input[0].ki.wVk = 0;
  input[0].ki.wScan = data;
  input[0].ki.dwFlags = 0x4;//KEYEVENTF_UNICODE;
 
  input[1].type = INPUT_KEYBOARD;
  input[1].ki.wVk = 0;
  input[1].ki.wScan = data;
  input[1].ki.dwFlags = KEYEVENTF_KEYUP | 0x4;//KEYEVENTF_UNICODE;
 
  SendInput(1, input, sizeof(INPUT));
}

void SendKeys(CString msg)
{
  short vk;
  BOOL shift;

  USES_CONVERSION;
  wchar_t* data = T2W(msg.GetBuffer(0));
  int len = wcslen(data);

  for(int i=0;i<len;i++)
  {
    if (data[i]>=0 && data[i]<256) //ascii字符
    {
      vk = VkKeyScanW(data[i]);

      if (vk == -1)
      {
        SendUnicode(data[i]);
      }
      else
      {
        if (vk < 0)
        {
          vk = ~vk + 0x1;
        }
       
        shift = vk >> 8 & 0x1;
       
        if (GetKeyState(VK_CAPITAL) & 0x1)
        {
          if (data[i]>='a' && data[i]<='z' || data[i]>='A' && data[i]<='Z')
          {
            shift = !shift;
          }
        }

        SendAscii(vk & 0xFF, shift);
      }
    }
    else //unicode字符
    {
      SendUnicode(data[i]);
    }
  }
}

void SendKeysEx(LPCTSTR str)
{
	short vk;
	BOOL shift;

	USES_CONVERSION;
	WCHAR *data = T2W(str);
	int len = wcslen(data);
	
	for(int i = 0; i < len; ++i)
	{
		SendUnicode(data[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTestSendInputDlg dialog

CTestSendInputDlg::CTestSendInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestSendInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestSendInputDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestSendInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestSendInputDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestSendInputDlg, CDialog)
	//{{AFX_MSG_MAP(CTestSendInputDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestSendInputDlg message handlers

BOOL CTestSendInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	TCHAR bufFile[MAX_PATH] = {0};
	GetModuleFileNameRe(NULL, TEXT("log"), bufFile, sizeof(bufFile) / sizeof(TCHAR));
	m_pLog = new CLog(bufFile, TRUE, 0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestSendInputDlg::OnPaint() 
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
HCURSOR CTestSendInputDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestSendInputDlg::OnOK() 
{
	// TODO: Add extra validation here
	//ShellExecute(NULL, NULL, "notepad.exe", NULL, NULL, SW_SHOWNORMAL);
	
	Sleep(500); //为了确保记事本程序打开完毕，稍等片刻
	
	CWnd *pWnd = FindWindow(NULL, "无标题 - 记事本");
	if (pWnd)
	{
		pWnd->SetForegroundWindow();
		//Sleep(1000);
		SendKeys("ab叶道全");
	}
	//CDialog::OnOK();
}
