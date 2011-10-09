
// DBSchemaTableView.h : CDBSchemaTableView 类的接口
//

#pragma once

#include "GridCtrl.h"
#include "Layout/FlowLayout.h"
#include "Layout/CtrlLayout.h"

class CDBSchemaTableView : public CView
{
protected: // 仅从序列化创建
	CDBSchemaTableView();
	DECLARE_DYNCREATE(CDBSchemaTableView)

// 属性
public:
	CDBSchemaMaintainerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDBSchemaTableView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CGridCtrl					Grid_;
	NSYedaoqLayout::CFlowLayout Layout_;
	CComboBox					CmbTables_;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DBSchemaTableView.cpp 中的调试版本
inline CDBSchemaMaintainerDoc* CDBSchemaTableView::GetDocument() const
   { return reinterpret_cast<CDBSchemaMaintainerDoc*>(m_pDocument); }
#endif

