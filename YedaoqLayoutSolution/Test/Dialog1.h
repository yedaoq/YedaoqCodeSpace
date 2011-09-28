#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "..\YedaoqLayout\Layout.h"
#include "..\YedaoqLayout\FlowLayout.h"
#include "..\YedaoqLayout\CtrlLayout.h"
#include "..\YedaoqLayout\ZoomBlankLayout.h"

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CDialog1 对话框

class CDialog1 : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDialog1)

public:
	CDialog1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialog1();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOG1, IDH = IDR_HTML_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	CButton m_BtnA;
	CButton m_BtnB;
	CButton m_BtnX;
	CButton m_BtnY;
	CTreeCtrl m_Tre1;
	CListCtrl m_Lst1;

	NSYedaoqLayout::CFlowLayout m_GUILayout;
	
};
