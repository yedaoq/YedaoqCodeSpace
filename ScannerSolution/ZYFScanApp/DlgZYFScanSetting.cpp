// DlgZYFScanSetting.cpp : implementation file
//

#include "stdafx.h"
#include "ZYFScanApp.h"
#include "DlgZYFScanSetting.h"
#include "afxdialogex.h"
#include "Helper.h"

// CDlgZYFScanSetting dialog

IMPLEMENT_DYNAMIC(CDlgZYFScanSetting, CDialogEx)

CDlgZYFScanSetting::CDlgZYFScanSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgZYFScanSetting::IDD, pParent), m_GridBatchRow(-1)
{

}

CDlgZYFScanSetting::~CDlgZYFScanSetting()
{
}

void CDlgZYFScanSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRIDBATCH, m_GridBatch);
	DDX_Control(pDX, IDC_GRDIITEM, m_GridItem);
}

BEGIN_MESSAGE_MAP(CDlgZYFScanSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTADDBATCH, &CDlgZYFScanSetting::OnBnClickedButaddbatch)
	ON_BN_CLICKED(IDC_BTNDELBATCH, &CDlgZYFScanSetting::OnBnClickedBtndelbatch)
	ON_BN_CLICKED(IDC_BTNADDITEM, &CDlgZYFScanSetting::OnBnClickedBtnadditem)
	ON_BN_CLICKED(IDC_BTNDELITEM, &CDlgZYFScanSetting::OnBnClickedBtndelitem)
	ON_BN_CLICKED(IDOK, &CDlgZYFScanSetting::OnBnClickedOk)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRIDBATCH, &CDlgZYFScanSetting::OnGridBatchSelChanged)
END_MESSAGE_MAP()

void CDlgZYFScanSetting::Initialize()
{
	m_GridBatch.SetRowCount(0);
	m_GridBatch.SetColumnCount(1);

	m_GridBatch.SetFixedRowCount(0);
	m_GridBatch.SetColumnWidth(0, 120);

	m_GridItem.SetRowCount(1);
	m_GridItem.SetColumnCount(4);

	m_GridItem.SetFixedRowCount(1);
	m_GridItem.SetColumnWidth(0, 90);
	m_GridItem.SetColumnWidth(1, 60);
	m_GridItem.SetColumnWidth(2, 60);
	m_GridItem.SetColumnWidth(3, 60);

	m_GridItem.SetItemText(0, 0, TEXT("名称"));
	m_GridItem.SetItemText(0, 1, TEXT("扫描坐标"));
	m_GridItem.SetItemText(0, 2, TEXT("扫描尺寸"));
	m_GridItem.SetItemText(0, 3, TEXT("显示位置"));
}

BOOL CDlgZYFScanSetting::OnInitDialog()
{
	if(!__super::OnInitDialog())
	{
		return FALSE;		
	}

	Initialize();
	Load(CZYFScanOptions::GetInstance());
}

void CDlgZYFScanSetting::Load(const CZYFScanOptions& options )
{
	m_Options = options;

	m_GridBatch.SetRowCount(m_Options.ScanBatchs.size());

	for (int i = 0; i < m_Options.ScanBatchs.size(); ++i)
	{
		m_GridBatch.SetItemText(i, 0, m_Options.ScanBatchs[i].Name.c_str());
	}
	m_GridBatch.Invalidate();
}
// CDlgZYFScanSetting message handlers


void CDlgZYFScanSetting::OnBnClickedButaddbatch()
{
	// TODO: Add your control notification handler code here
	m_GridBatch.InsertRow(TEXT(""));
	m_Options.ScanBatchs.push_back(CScanBatch());
	m_GridBatch.Invalidate();
}


void CDlgZYFScanSetting::OnBnClickedBtndelbatch()
{
	// TODO: Add your control notification handler code here
	CCellRange range = m_GridBatch.GetSelectedCellRange();
	if(!range.IsValid())
	{
		return;
	}
	tstring str = m_GridBatch.GetCell(range.GetMinRow(), 0)->GetText();
	if(!str.empty())
	{
		if(EnumMessageBoxResult::MBROK !=  MsgboxConfirm(TEXT("确认要删除扫描批次《%s》吗？"), str.c_str()))
		{
			return;
		}
	}
	
	m_GridBatch.DeleteRow(range.GetMinRow());
	m_Options.ScanBatchs.erase(m_Options.ScanBatchs.begin() + range.GetMinRow());
	m_GridBatch.Invalidate();
}


void CDlgZYFScanSetting::OnBnClickedBtnadditem()
{
	// TODO: Add your control notification handler code here
	m_GridItem.InsertRow(TEXT(""));
	m_GridItem.Invalidate();
}


void CDlgZYFScanSetting::OnBnClickedBtndelitem()
{
	// TODO: Add your control notification handler code here
	CCellRange range = m_GridItem.GetSelectedCellRange();
	if(!range.IsValid())
	{
		return;
	}
	tstring str = m_GridItem.GetCell(range.GetMinRow(), 0)->GetText();
	if(!str.empty())
	{
		if(EnumMessageBoxResult::MBROK !=  MsgboxConfirm(TEXT("确认要删除扫描批次《%s》吗？"), str.c_str()))
		{
			return;
		}
	}

	m_GridItem.DeleteRow(range.GetMinRow());
	m_GridItem.Invalidate();
}

void CDlgZYFScanSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//m_GridBatch.SetSelectedRange(m_GridBatchRow, 0, m_GridBatchRow, 0, FALSE);
	OnGridBatchSelChanged(NULL, NULL);

	for (int i = 0; i < m_Options.ScanBatchs.size(); ++i)
	{
		m_Options.ScanBatchs[i].Name = m_GridBatch.GetCell(i, 0)->GetText();
	}

	CDialogEx::OnOK();
}

void CDlgZYFScanSetting::OnGridBatchSelChanged(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	//NM_GRIDVIEW* pNm = (NM_GRIDVIEW*)pNotifyStruct;
	CCellRange range = m_GridBatch.GetSelectedCellRange();
	
	if(m_GridBatchRow >= 0 && m_GridBatchRow < m_GridBatch.GetRowCount())
	{
		TTRACE(TEXT("Save Row : %d\n"), m_GridBatchRow);
		//int rowPre = pNm->iRow - 1;
		CScanBatch& batchPre = m_Options.ScanBatchs[m_GridBatchRow];
		batchPre.Items.clear();
		for (int i = 1; i < m_GridItem.GetRowCount(); ++i)
		{
			ScanItem item;
			item.Name = m_GridItem.GetCell(i,0)->GetText();
			item.ScanPos = FromStr<CPoint>(tstring(m_GridItem.GetCell(i,1)->GetText()));
			item.ScanSize = FromStr<CPoint>(tstring(m_GridItem.GetCell(i,2)->GetText()));
			item.ShowPos = FromStr<CPoint>(tstring(m_GridItem.GetCell(i,3)->GetText()));
			batchPre.Items.push_back(item);
		}
	}

	m_GridItem.SetRowCount(1);
	if(range.IsValid() && range.GetMinRow() >= 0)
	{
		m_GridBatchRow = range.GetMinRow();
		TTRACE(TEXT("Load Row : %d\n"), m_GridBatchRow);
		//int rowCur = range.GetMinRow() - 1;
		CScanBatch& batchCur = m_Options.ScanBatchs[m_GridBatchRow];
		m_GridItem.SetRowCount(batchCur.Items.size() + 1);
		for (int i = 0; i < batchCur.Items.size(); ++i)
		{
			m_GridItem.GetCell(i + 1,0)->SetText(batchCur.Items[i].Name.c_str());
			m_GridItem.GetCell(i + 1,1)->SetText(ToStr(batchCur.Items[i].ScanPos).c_str());
			m_GridItem.GetCell(i + 1,2)->SetText(ToStr(batchCur.Items[i].ScanSize).c_str());
			m_GridItem.GetCell(i + 1,3)->SetText(ToStr(batchCur.Items[i].ShowPos).c_str());
		}
	}

}