
// Dwarves.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Dwarves.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DwarfDoc.h"
#include "DwarfView.h"
#include "DBInterface\DBSourcePath.h"
#include "DBInterface\DBDataAdapter.h"
//#include "DBNameMappingLP.h"
#include "SqliteNameMapping.h"
#include "SqliteSource.h"
#include "LocalPlayerObjEnum.h"
#include "CommandIDAlloter.h"
#include <Helper.h>
#include "DBModuleLP.h"

using namespace NSDBModule;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDwarvesApp

BEGIN_MESSAGE_MAP(CDwarvesApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDwarvesApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CDwarvesApp::OnFileOpen)
	ON_COMMAND_RANGE(MinMenuViewID, MaxMenuViewID, &CDwarvesApp::OnViewOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDwarvesApp 构造

CDwarvesApp::CDwarvesApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CDwarvesApp 对象

CDwarvesApp theApp;


// CDwarvesApp 初始化

BOOL CDwarvesApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_DwarvesTYPE,
		RUNTIME_CLASS(CDwarfDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CDwarfView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_DwarvesTYPE))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if(cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew)
	{
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//初始化唯一的文档对象
	CDwarfDoc* pDoc = CGlobalData::GetCurrentDoc();
	pDoc->m_bAutoDelete = FALSE;
	pDoc->SetTitle(TEXT("请选择数据库"));
	pDoc->SetDBModule(&CDBModuleLP::GetInstance());
	pDocTemplate->AddDocument(pDoc);

	return TRUE;
}

CDwarfView* CDwarvesApp::GetExistedViewByID(int id)
{
	CDwarfDoc* pDoc = CGlobalData::GetCurrentDoc();
	POSITION pos = pDoc->GetFirstViewPosition();
	while(pos)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(!pView) continue;
		CDwarfView* pDwarfView = dynamic_cast<CDwarfView*>(pView);
		if(pDwarfView && pDwarfView->GetViewID() == id)
		{
			return pDwarfView;
		}
	}

	return 0;
}

CDwarfView* CDwarvesApp::NewView(int id)
{
	CDwarfDoc*		pDoc		= CGlobalData::GetCurrentDoc();
	CDocTemplate*	pTemplate	= pDoc->GetDocTemplate();
	
	ASSERT_VALID(pTemplate);

	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDoc, 0);
	if (pFrame == NULL)
	{
		TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
		return 0;     // command failed
	}

	pTemplate->InitialUpdateFrame(pFrame, pDoc);

	CDwarfView* pView = (CDwarfView*)pFrame->GetActiveView();
	if(pView)
	{
		TTRACE(TEXT("GetActiveView Successed~\r\n"));
		pView->SetViewID(id);
	}

	return pView;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CDwarvesApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CDwarvesApp::OnFileOpen()
{
	CSqliteSourceManager smr;
	std::auto_ptr<IDBConnection> pConn(smr.PromptSelectDBSource(0));
	IDBDataAdapter* PAdapter;
	IDBFactory*		pFactory;
	IDBNameMapping* pMapping = new CSqliteNameMapping();

	if(!pConn.get() || !smr.OpenDBConnection(pConn.get(), &PAdapter, &pFactory))
	{
		return;
	}

	CDBModuleLP::GetInstance().AttachToDatabase(PAdapter, pFactory, pMapping);
	CDBModuleLP::GetInstance().RefreshData();

	CGlobalData::GetCurrentDoc()->SetTitle(pConn->ToString().c_str());

	RegDeleteKey(HKEY_CURRENT_USER, TEXT("Software\\应用程序向导生成的本地应用程序\\Dwarves"));

	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	pMainFrame->ClearDwarfViewmenuList();
	pMainFrame->OnSetMenu(pMainFrame->GetMenuBar()->GetHMenu());
}

void CDwarvesApp::OnViewOpen(UINT id)
{
	int viewID = id - MinMenuViewID;

	CDwarfView* pView = GetExistedViewByID(viewID);

	if(!pView)
	{
		pView = NewView(viewID);
	}
	else
	{
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
		pMainFrame->MDIActivate(pView->GetParentFrame());

		//CMFCTabCtrl& tab = pMainFrame->GetMDITabs();
		//tab.SetActiveTab(tab.GetActiveTab());
		//tab.EnableActiveTabCloseButton(FALSE);

		/*CChildFrame* pChildFrame = static_cast<CChildFrame*>(pView->GetParentFrame());
		pChildFrame->SetActiveView(pView, TRUE);
		pChildFrame->MDIActivate();		*/
	}
}

// CDwarvesApp 自定义加载/保存方法

void CDwarvesApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CDwarvesApp::LoadCustomState()
{
}

void CDwarvesApp::SaveCustomState()
{
}

// CDwarvesApp 消息处理程序



