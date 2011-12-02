
// DwarfView.h : CDwarfView 类的接口
//


#pragma once
#include "DwarfDoc.h"
#include <Layout\FlowLayout.h>
#include <GridCtrl.h>
#include "DBTableViewer4GridCtrl.h"
#include "DwarfViewInfo.h"
#include "GridSelectedRecordsEnumerator.h"

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
	int			GetViewID()				{return ViewID; }
	void		SetViewID(int id);

	void		ResetTitle();

	int			Initialize();
	int			ShowRecords();

	int			RemoveRecordUpdated();
	int			RemoveRecordFocused();
	int			RemoveRecordSelected();
	
	int			AddRecord(const IDBRecord& rec);

	IDBRecord*				GetFocusedRecord();
	IEnumerator<IDBRecord>*	GetSelectedRecords();

	int					GetUpdatedRecord(IDBRecord* cur, IDBRecord* ori);

	void CreateButton(CButton& btn, UINT id, CWnd* pParent, LPCTSTR lpTitle = NULL, UINT width = 75, UINT height = 23, DWORD dwStyle = WS_CHILD | BS_CENTER | BS_VCENTER | BS_TEXT | BS_PUSHBUTTON, CFont* font = NULL);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
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

	CGridCtrl					Grid;
	CGridCtrl					GridEdit;
	OpBtnCollection				BtnOps;
	CButton						BtnSearch;

	CDBTableViewer4GridCtrl		GridViewer;
	CDBTableViewer4GridCtrl		GridEditViewer;

	DwarfViewOperationContext	Context;

	CDBRecordAuto				RecordFocused;
	int							RecordIdxFocused;

	CDBRecordAuto				RecordUpdated;
	int							RecordIdxUpdated;

	CGridSelectedRecordsEnumerator SelectedRecords;

	static bool					BkColorInitFlag;


// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnViewOperation(UINT id);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGridSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridSelDBClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DwarfView.cpp 中的调试版本
inline CDwarfDoc* CDwarfView::GetDocument() const
   { return reinterpret_cast<CDwarfDoc*>(m_pDocument); }
#endif

