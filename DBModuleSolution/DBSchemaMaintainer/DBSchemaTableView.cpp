
// DBSchemaTableView.cpp : CDBSchemaTableView 类的实现
//

#include "stdafx.h"
#include "DBSchemaMaintainer.h"
#include "DBSchemaMaintainerDoc.h"
#include "DBSchemaTableView.h"
#include "DBSchemaInfoRecord.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NSYedaoqLayout;

// CDBSchemaTableView

IMPLEMENT_DYNCREATE(CDBSchemaTableView, CView)

BEGIN_MESSAGE_MAP(CDBSchemaTableView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDBSchemaTableView::OnFilePrintPreview)
	ON_COMMAND(EIDC_BTNMERGE, &CDBSchemaTableView::OnBtnMergeClicked)
	ON_NOTIFY(GVN_SELCHANGED, EIDC_GRIDTBL, &CDBSchemaTableView::OnGridTblSelChanged)
	ON_NOTIFY(GVN_SELCHANGED, EIDC_GRIDCOL, &CDBSchemaTableView::OnGridColSelChanged)
	ON_NOTIFY(GVN_ENDLABELEDIT, EIDC_GRIDCOL, &CDBSchemaTableView::OnGridColTxtChanged)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDBSchemaTableView 构造/析构

CDBSchemaTableView::CDBSchemaTableView()
	: Layouter(EnumLayoutDirection::Vertical), 
	GridColViewer(&GridCol, 1), 
	GridTabViewer(&GridTab, 1),
	CurrentTableIndex(-1)
{
	// TODO: 在此处添加构造代码

	Grid_Select = CDBColumnViewInfo(
		TEXT(""), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		20,
		false);

	GridTab_Name = CDBColumnViewInfo(
		TEXT("名称"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		140,
		true);

	GridTab_DBName = CDBColumnViewInfo(
		TEXT("数据库名"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		140,
		true);

	GridTab_BuildIn = CDBColumnViewInfo(
		TEXT("内置"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		35,
		true);

	GridTab_DBExist = CDBColumnViewInfo(
		TEXT("存在"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		35,
		true);

	GridTab_State = CDBColumnViewInfo(
		TEXT(""), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		20,
		true);

	GridTblColumns.AppendVirtual(&Grid_Select);
	GridTblColumns.Append(&GridTab_State);
	GridTblColumns.Append(&GridTab_Name);
	GridTblColumns.Append(&GridTab_DBName);
	GridTblColumns.Append(&GridTab_BuildIn);
	GridTblColumns.Append(&GridTab_DBExist);	

	GridCol_State = CDBColumnViewInfo(
		TEXT(""), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		20,
		true);

	GridCol_Name = CDBColumnViewInfo(
		TEXT("名称"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		120,
		true);

	GridCol_DBName = CDBColumnViewInfo(
		TEXT("数据库名"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		120,
		true);

	GridCol_Buildin = CDBColumnViewInfo(
		TEXT("内置"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		35,
		true);

	GridCol_DBExist = CDBColumnViewInfo(
		TEXT("存在"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		35,
		true);

	GridCol_CppType = CDBColumnViewInfo(
		TEXT("类型"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		80,
		false);

	GridCol_DBType = CDBColumnViewInfo(
		TEXT("数据库类型"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleNone::GetInstance(),
		80,
		true);

	GridCol_KeyCol = CDBColumnViewInfo(
		TEXT("关键列"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		false);

	GridCol_DBPK = CDBColumnViewInfo(
		TEXT("主键"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		true);

	GridCol_DBNull = CDBColumnViewInfo(
		TEXT("可空"), 
		&CTextFormatSwitcherNone::GetInstance(),
		&CEditStyleBool::GetInstance(),
		40,
		true);

	GridCol_RelyTbl = CDBColumnViewInfo(
		TEXT("外表"),
		&GridCol_RelyTblFormat,
		&GridCol_RelyTblStyle,
		140,
		false);

	GridCol_RelyCol = CDBColumnViewInfo(
		TEXT("外键"),
		&GridCol_RelyColFormat,
		&GridCol_RelyColStyle,
		120,
		false);

	GridCol_VisiCol = CDBColumnViewInfo(
		TEXT("视图列"),
		&GridCol_RelyColFormat,
		&GridCol_RelyColStyle,
		120,
		false);

	GridColColumns.AppendVirtual(&Grid_Select);
	GridColColumns.Append(&GridCol_State);
	GridColColumns.Append(&GridCol_Name);
	GridColColumns.Append(&GridCol_DBName);
	GridColColumns.Append(&GridCol_Buildin);
	GridColColumns.Append(&GridCol_DBExist);
	GridColColumns.Append(&GridCol_CppType);
	GridColColumns.Append(&GridCol_DBType);
	GridColColumns.Append(&GridCol_KeyCol);
	GridColColumns.Append(&GridCol_DBPK);
	GridColColumns.Append(&GridCol_DBNull);
	GridColColumns.Append(&GridCol_RelyTbl);
	GridColColumns.Append(&GridCol_RelyCol);
	GridColColumns.Append(&GridCol_VisiCol);
}

CDBSchemaTableView::~CDBSchemaTableView()
{
}

BOOL CDBSchemaTableView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	//cs.style ^= WS_BORDER;

	return CView::PreCreateWindow(cs);
}

int CDBSchemaTableView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
		return -1;

	RECT rect = {0, 0, lpcs->cx, lpcs->cy};
	GridCol.Create(rect, this, EIDC_GRIDCOL, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
	GridTab.Create(rect, this, EIDC_GRIDTBL, WS_CHILD | WS_TABSTOP | WS_VISIBLE);

	CreateButton(BtnMerge, EIDC_BTNMERGE, this, TEXT("合并"));

	//RECT rect1 = { 0, 0, 150, 28 };
	//CmbTab.Create(CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP, rect1, this, 3);
	//CmbTab.ShowWindow(SW_SHOW);

	CFlowLayout* pFlow = Layouter.AddFlow(EnumLayoutDirection::Horizon);
	pFlow->AddCtrl(BtnMerge.GetSafeHwnd());

	pFlow = Layouter.AddFlow(
		EnumLayoutDirection::Horizon,
		ResizeInfo::FillInfo);

	pFlow->AddCtrl(
		GridTab.GetSafeHwnd(),
		ResizeInfo(EnumResizeMode::Fixed, 400),
		ResizeInfo::FillInfo);

	pFlow->AddCtrl(
		GridCol.GetSafeHwnd(), 
		ResizeInfo::FillInfo, 
		ResizeInfo::FillInfo);

	GridTab.SetColumnCount(6);
	GridTabViewer.Initialize(GridTblColumns);

	GridCol.SetColumnCount(14);
	GridColViewer.Initialize(GridColColumns);

	return S_OK;
}

// CDBSchemaTableView 绘制

void CDBSchemaTableView::OnDraw(CDC* /*pDC*/)
{
	CDBSchemaMaintainerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CDBSchemaTableView 打印


void CDBSchemaTableView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CDBSchemaTableView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDBSchemaTableView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDBSchemaTableView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDBSchemaTableView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDBSchemaTableView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

afx_msg void CDBSchemaTableView::OnSize(UINT nType, int cx, int cy)
{
	Layouter.Layout(LayoutPoint(0, 0), LayoutSize(cx, cy));
}

// CDBSchemaTableView 诊断

#ifdef _DEBUG
void CDBSchemaTableView::AssertValid() const
{
	CView::AssertValid();
}

void CDBSchemaTableView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDBSchemaMaintainerDoc* CDBSchemaTableView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDBSchemaMaintainerDoc)));
	return (CDBSchemaMaintainerDoc*)m_pDocument;
}
#endif //_DEBUG

void CDBSchemaTableView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CDBModule& module = GetDocument()->GetDBModule();
	
	DBTblNameEnumerator = std::auto_ptr<IEnumerator<tstring>>(module.Tables().EnumName());
	GridCol_RelyTblStyle.Options = DBTblNameEnumerator.get();
	GridCol_RelyTblFormat = CTextConverter4DBTblNameIdx(&module.Tables());	

	GridCol_RelyColFormat.SetModule(&GetDocument()->GetDBModule());
	
	CurrentTableIndex = -1;

	GridTabViewer.Clear();
	GridColViewer.Clear();
	CDBTableInfoEnumerator pEnumTbl(&module);
	GridTabViewer.Fill(pEnumTbl);
}


// CDBSchemaTableView 消息处理程序
void CDBSchemaTableView::OnGridColSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CCellRange range = GridCol.GetSelectedCellRange();

	//TTRACE(TEXT("Row Select - %d\r\n"), range.GetMinRow());

	if(!range.IsValid() || range.GetMinRow() < GRIDHEADERROWCOUNT)
	{
		return;
	}

	CDBModule&		module = GetDocument()->GetDBModule();
	CDBRecordAuto	rec;
	GridColViewer.GetCurRecord(&rec); 


	tstring strRelayTblID = rec.GetField(CDBColumnInfoRecord::RelyTbl);
	int relyTblID = -1;
	if(!strRelayTblID.empty())
	{
		relyTblID = boost::lexical_cast<int>(rec.GetField(CDBColumnInfoRecord::RelyTbl));
	}

	TTRACE(TEXT("依赖表 : %d\r\n"), relyTblID);
	if( relyTblID >= 0)
	{
		GridCol_RelyColStyle.SetTable(&GetDocument()->GetDBModule().Tables()[relyTblID]->GetSchema());
	}
}

void CDBSchemaTableView::OnGridTblSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	CCellRange range = GridTab.GetSelectedCellRange();
	if(range.IsValid() && range.GetMinRow() >= GRIDHEADERROWCOUNT)
	{
		ShowColumnsOfTable(range.GetMinRow() - GRIDHEADERROWCOUNT);
	}
}

void CDBSchemaTableView::OnGridColTxtChanged(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if(pItem->iRow < GRIDHEADERROWCOUNT || pItem->iRow >= GridCol.GetRowCount())
	{
		return;
	}

	CDBTableCollection& tables = GetDocument()->GetDBModule().Tables();
	if(CurrentTableIndex < 0 || CurrentTableIndex >= tables.Count())
	{
		_ASSERT(FALSE);
		return;
	}

	CDBColumnInfoRecord col(&tables[CurrentTableIndex]->GetSchema()[pItem->iRow - GRIDHEADERROWCOUNT]);
	
	CDBRecordAuto rec;
	GridColViewer.GetRecordAt(pItem->iRow - GRIDHEADERROWCOUNT, &rec);

	int fieldIdx = GridColColumns.GetColumnByViewCol(pItem->iColumn)->IdxRecord;
	col.SetField(fieldIdx, rec.GetField(fieldIdx));

	TTRACE(TEXT("Column %s : field %d changed to %s\r\n"), col.GetField(CDBColumnInfoRecord::Name).c_str(), fieldIdx, col.GetField(fieldIdx));

	//TRACE("Grid Edited...\r\n");
}

void CDBSchemaTableView::OnBtnMergeClicked()
{
	int iSelectedRowCount = 0;
	int iSelectedRows[2];

	for(int i = GRIDHEADERROWCOUNT; i < GridTab.GetRowCount(); ++i)
	{
		CGridCellBase* pCell = GridTab.GetCell(i, 0);
		if(CEditStyleBool::GetInstance().strTrue == pCell->GetText())
		{
			iSelectedRows[iSelectedRowCount++] = i - GRIDHEADERROWCOUNT;
			if(iSelectedRowCount >= 2) break;
		}
	}

	if(iSelectedRowCount == 2)
	{
		GetDocument()->MergeTable(iSelectedRows[0], iSelectedRows[1]);
	}

	iSelectedRowCount = 0;
	for(int i = GRIDHEADERROWCOUNT; i < GridCol.GetRowCount(); ++i)
	{
		CGridCellBase* pCell = GridCol.GetCell(i, 0);
		TTRACE(pCell->GetText());
		if(CEditStyleBool::GetInstance().strTrue == pCell->GetText())
		{
			iSelectedRows[iSelectedRowCount++] = i - GRIDHEADERROWCOUNT;
			if(iSelectedRowCount >= 2) break;
		}
	}

	if(iSelectedRowCount == 2)
	{
		GetDocument()->MergeColumn(GridTabViewer.GetCurRecord(0), iSelectedRows[0], iSelectedRows[1]);
	}
}

int CDBSchemaTableView::ShowColumnsOfTable(int idxTbl)
{
	CDBTableCollection& tables = GetDocument()->GetDBModule().Tables();

	_ASSERT(idxTbl >= 0 && idxTbl < tables.Count());

	CurrentTableIndex = idxTbl;
	GridCol_RelyColFormat.SetTable(idxTbl);
	
	GridColViewer.Clear();
	GridColViewer.Fill(CDBColumnInfoEnumerator(&tables[idxTbl]->GetSchema()));
	return 1;
}

void CDBSchemaTableView::CreateButton(CButton& btn, UINT id, CWnd* pParent, LPCTSTR lpTitle, UINT width, UINT height, DWORD dwStyle, CFont* pFont)
{
	static CFont* pDefaultFont = 0;

	if(!pDefaultFont)
	{
		pDefaultFont = new CFont;
		pDefaultFont->CreateFont(16,		//   nHeight
			0,								//   nWidth
			0,								//   nEscapement
			0,								//   nOrientation
			0,								//   nWeight
			FALSE,							//   bItalic
			FALSE,							//   bUnderline
			0,								//   cStrikeOut
			ANSI_CHARSET,					//   nCharSet
			OUT_DEFAULT_PRECIS,				//   nOutPrecision
			CLIP_DEFAULT_PRECIS,			//   nClipPrecision
			DEFAULT_QUALITY,				//   nQuality
			DEFAULT_PITCH   |   FF_SWISS,	//   nPitchAndFamily
			_T( "Arial "));					//   lpszFac 
	}

	RECT rect = {0, 0, width, height};
	btn.Create(lpTitle, dwStyle, rect, pParent, id);
	if(!pFont) pFont = pDefaultFont;
	btn.SetFont(pFont);
	btn.ShowWindow(SW_SHOW);
}