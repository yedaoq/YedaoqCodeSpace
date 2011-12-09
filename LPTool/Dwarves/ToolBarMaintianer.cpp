#include "StdAfx.h"
#include "ToolBarMaintianer.h"
#include "CommandIDAlloter.h"

CToolBarMaintianer::CToolBarMaintianer(void)
	: m_ToolCmbSearch(ToolBarCmbSearchID, -1, CBS_DROPDOWN, 100),
	  m_ToolBtnFindNext(ToolBarBtnFindNextID, -1, TEXT("Next"))
{
	m_ToolCmbSearch.AddItem(TEXT("ffdshow"));
	m_ToolCmbSearch.AddItem(TEXT("mpc"));

	//m_ToolCmbSearch.EnableWindow(TRUE);
	//m_ToolCmbSearch.SetCenterVert();
	//m_ToolCmbSearch.SetDropDownHeight(100);
	//m_ToolCmbSearch.SetFlatMode();
	//m_ToolCmbSearch.SetStyle();
	m_ToolCmbSearch.SetText(TEXT("Find..."));
	//m_ToolCmbSearch.m_nID = ToolBarCmbSearchID;

	//m_ToolBtnFindNext.EnableWindow(TRUE);
	//m_ToolBtnFindNext.m_strText = TEXT("Next");
	//m_ToolBtnFindNext.m_bImage = FALSE;
	//m_ToolBtnFindNext.m_bText = TRUE;
	//m_ToolBtnFindNext.m_nID = ToolBarBtnFindNextID;
}

CToolBarMaintianer::~CToolBarMaintianer(void)
{
}

int CToolBarMaintianer::Clean()
{
	m_Bar->RemoveAllButtons();
	return 1;
}

int CToolBarMaintianer::InitBasicItems()
{
	m_Bar->InsertButton(m_ToolCmbSearch);
	m_Bar->InsertButton(m_ToolBtnFindNext);

	return 1;
}

int CToolBarMaintianer::SetToolBar( CMFCToolBar* bar )
{
	m_Bar = bar;
	return 1;
}
