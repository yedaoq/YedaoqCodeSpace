
// DwarfView.h : CDwarfView 类的接口
//


#pragma once
#include "DwarfDoc.h"
#include <Layout\FlowLayout.h>
#include <GridCtrl.h>
#include "DBTableViewer4GridCtrl.h"

class CDwarfView : public CView
{
public:
	typedef std::vector<CButton*> OpBtnCollection;

protected: // 仅从序列化创建
	CDwarfView();
	DECLARE_DYNCREATE(CDwarfView)

// 属性
public:
	CDwarfDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual int GetViewID() {return ViewID; }
	virtual void SetViewID(int id);

	virtual void ResetTitle();

	virtual int Initialize();
	virtual int ShowRecords();

	void	CreateButton(CButton& btn, UINT id, CWnd* pParent, LPCTSTR lpTitle = NULL, UINT width = 75, UINT height = 23, DWORD dwStyle = WS_CHILD | BS_CENTER | BS_VCENTER | BS_TEXT | BS_PUSHBUTTON, CFont* font = NULL);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// 实现
public:
	virtual ~CDwarfView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	enum {EIDC_GRID = 0, EIDC_BTNSEARCH, EIDC_UNUSED };

protected:
	int	ViewID;

	NSYedaoqLayout::CFlowLayout FlowLayoutMain;

	OpBtnCollection				BtnOps;
	CGridCtrl					Grid;
	CButton						BtnSearch;
	CDBTableViewer4GridCtrl		GridViewer;


// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnViewOperation(UINT id);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DwarfView.cpp 中的调试版本
inline CDwarfDoc* CDwarfView::GetDocument() const
   { return reinterpret_cast<CDwarfDoc*>(m_pDocument); }
#endif

