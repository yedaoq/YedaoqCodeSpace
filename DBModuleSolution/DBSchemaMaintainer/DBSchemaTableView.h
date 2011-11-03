
// DBSchemaTableView.h : CDBSchemaTableView 类的接口
//

#pragma once

#include "GridCtrl.h"
#include "Layout/FlowLayout.h"
#include "Layout/CtrlLayout.h"
#include "Module/Schema/DBTableSchema.h"
#include "Module/DBTable.h"
#include "View/DBColumnViewInfo.h"
#include "DBTableViewer4GridCtrl.h"
#include "Module/Schema/DBSchemaValidater.h"
#include "TextConverter4DBTblNameIdx.h"
#include "DBSchemaInfoRecord.h"
#include <View/TextConverter4DBMap.h>
#include <TypeConverter.h>
#include <Module/DBRecordFunction.h>



class CDBSchemaTableView : public CView
{
public:
	class CRelyColEditStyle : CEditStyleOptional
	{
	public:
		void SetTable(NSDBModule::CDBTableSchema* sch)
		{
			if(sch)
			{
				ColumnNames = std::auto_ptr<IEnumerator<tstring>>(
					new_iterator_enumerator_ex<tstring>(sch->Columns.begin(), sch->Columns.end()));
			}
			else
			{
				
			}
			Options = ColumnNames.get();
		}

	protected:
		NSDBModule::CDBTableSchema*			TblPtr;
		std::auto_ptr<IEnumerator<tstring>>	ColumnNames;

	};

	class

protected: // 仅从序列化创建
	CDBSchemaTableView();
	DECLARE_DYNCREATE(CDBSchemaTableView)

// 属性
public:
	CDBSchemaMaintainerDoc* GetDocument() const;

	enum { EIDC_GRIDTBL = 1, EIDC_GRIDCOL, EIDC_BTNMERGE };
	enum { GRIDHEADERROWCOUNT = 1};

// 操作
public:
	int		ShowColumnsOfTable(int idxTbl);
	void	CreateButton(CButton& btn, UINT id, CWnd* pParent, LPCTSTR lpTitle = NULL, UINT width = 75, UINT height = 23, DWORD dwStyle = WS_CHILD | BS_CENTER | BS_VCENTER | BS_TEXT | BS_PUSHBUTTON, CFont* font = NULL);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// 实现
public:
	virtual ~CDBSchemaTableView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CGridCtrl					GridCol;
	CGridCtrl					GridTab;
	CGridCtrl					GridIdx;

	NSYedaoqLayout::CFlowLayout Layouter;
	CComboBox					CmbTab;
	CButton						BtnMerge;

	CDBTableViewColumnCollection GridTblColumns;
	CDBTableViewColumnCollection GridColColumns;

	CDBTableViewer4GridCtrl		GridColViewer;
	CDBTableViewer4GridCtrl		GridTabViewer;

	CDBColumnViewInfo			Grid_Select;

	CDBColumnViewInfo			GridTab_Name;
	CDBColumnViewInfo			GridTab_DBName;
	CDBColumnViewInfo			GridTab_BuildIn;
	CDBColumnViewInfo			GridTab_DBExist;
	CDBColumnViewInfo			GridTab_State;

	CDBColumnViewInfo			GridCol_State;
	CDBColumnViewInfo			GridCol_Name;
	CDBColumnViewInfo			GridCol_DBName;
	CDBColumnViewInfo			GridCol_Buildin;
	CDBColumnViewInfo			GridCol_DBExist;
	CDBColumnViewInfo			GridCol_CppType;
	CDBColumnViewInfo			GridCol_DBType;
	CDBColumnViewInfo			GridCol_KeyCol;
	CDBColumnViewInfo			GridCol_DBPK;
	CDBColumnViewInfo			GridCol_DBNull;
	CDBColumnViewInfo			GridCol_RelyTbl;
	CDBColumnViewInfo			GridCol_RelyCol;
	CDBColumnViewInfo			GridCol_VisiCol;

	CDBColumnViewInfo			GridIdx_Unique;
	CDBColumnViewInfo			GridIdx_Column;

	CDBColumnInfoEnumerator		DBColEnumerator;
	CConvertEnumerator<tstring, IDBRecord, CDBRecordToField> DBColIdxStrEnumerator;
	
	CRangeEnumerator<int>		DBTblIdxEnumerator;
	CConvertEnumeratorEx<tstring, int, CTypeConverter_LexicalCast<tstring, int>> DBTblIdxStrEnumerator;
	
	CEditStyleOptional			GridCol_RelyTblStyle;
	CTextConverter4DBTblNameIdx GridCol_RelyTblFormat;

	NSDBModule::CTextConverter4DBMap	GridCol_RelyColFormat;
	CEditStyleOptional			GridCol_RelyColStyle;

protected:

	int							CurrentTableIndex;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnGridTblSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridColSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridColTxtChanged(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBtnMergeClicked();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DBSchemaTableView.cpp 中的调试版本
inline CDBSchemaMaintainerDoc* CDBSchemaTableView::GetDocument() const
   { return reinterpret_cast<CDBSchemaMaintainerDoc*>(m_pDocument); }
#endif

