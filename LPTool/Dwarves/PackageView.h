
// PackageView.h : CPackageView ��Ľӿ�
//


#pragma once
#include "PackageDoc.h"
#include <Layout\FlowLayout.h>
#include <GridCtrl.h>
#include "DBTableViewer4GridCtrl.h"

class CPackageView : public CView
{
public:
	typedef std::vector<CButton*> OpBtnCollection;

protected: // �������л�����
	CPackageView();
	DECLARE_DYNCREATE(CPackageView)

// ����
public:
	CPackageDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
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

// ʵ��
public:
	virtual ~CPackageView();
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


// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnViewOperation(UINT id);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // PackageView.cpp �еĵ��԰汾
inline CPackageDoc* CPackageView::GetDocument() const
   { return reinterpret_cast<CPackageDoc*>(m_pDocument); }
#endif
