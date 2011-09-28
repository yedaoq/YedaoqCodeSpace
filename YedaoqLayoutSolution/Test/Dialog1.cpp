// Dialog1.cpp : 实现文件
//

#include "stdafx.h"
#include "Test.h"
#include "Dialog1.h"

using namespace NSYedaoqLayout;

// CDialog1 对话框

IMPLEMENT_DYNCREATE(CDialog1, CDHtmlDialog)

CDialog1::CDialog1(CWnd* pParent /*=NULL*/)
: CDHtmlDialog(CDialog1::IDD, CDialog1::IDH, pParent), m_GUILayout(EnumLayoutDirection::Horizon)
{

}

CDialog1::~CDialog1()
{
}

void CDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnA);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnB);
	DDX_Control(pDX, IDC_BUTTON3, m_BtnX);
	DDX_Control(pDX, IDC_BUTTON4, m_BtnY);
	DDX_Control(pDX, IDC_TREE1, m_Tre1);
	DDX_Control(pDX, IDC_LIST1, m_Lst1);
}

BOOL CDialog1::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	CFlowLayout* pFlow = m_GUILayout.AddFlow(
		EnumLayoutDirection::Vertical, 
		ResizeInfo(EnumResizeMode::Fixed, 200));

	CFlowLayout* pFlow1 = pFlow->AddFlow(EnumLayoutDirection::Horizon);
	
	pFlow1->AddCtrl(m_BtnA.GetSafeHwnd());
	pFlow1->AddCtrl(m_BtnB.GetSafeHwnd());

	pFlow->AddCtrl(
		m_Tre1.GetSafeHwnd(),
		ResizeInfo::FillInfo,
		ResizeInfo::FillInfo);
	
	pFlow = m_GUILayout.AddFlow(EnumLayoutDirection::Vertical);
	pFlow->AddZoomBlank(50,50);
	pFlow->AddCtrl(m_BtnX.GetSafeHwnd());
	pFlow->AddCtrl(m_BtnY.GetSafeHwnd());

	m_GUILayout.AddCtrl(
		m_Lst1.GetSafeHwnd(),
		ResizeInfo::FillInfo,
		ResizeInfo::FillInfo);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CDialog1, CDHtmlDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDialog1)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CDialog1 消息处理程序

HRESULT CDialog1::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CDialog1::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CDialog1::OnSize(UINT nType, int cx, int cy)
{
	CDHtmlDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_GUILayout.Layout(LayoutPoint(3, 3), LayoutSize(cx - 6, cy - 6));
	Invalidate();
}
