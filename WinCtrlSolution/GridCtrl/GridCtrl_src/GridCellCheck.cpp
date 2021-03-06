// GridCellCheck.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the
// grid control.
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// CInPlaceList from http://www.codeguru.com/listview
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22+
//
// History:
// 23 Jul 2001 - Complete rewrite
// 13 Mar 2004 - GetCellExtent and GetCheckPlacement fixed by Yogurt
//             - Read-only now honoured - Yogurt
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCell.h"
#include "GridCtrl.h"
#include "GridCellCheck.h"
#include "CellTemplate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "EditStyle.h"

IMPLEMENT_DYNCREATE(CGridCellCheck, CGridCell)

CGridCellCheck::CGridCellCheck() : CGridCell()
{
    m_bChecked = FALSE;
    //m_Rect.IsRectNull();
}

void CGridCellCheck::SetText(LPCTSTR szVal)
{
	ASSERT(m_Template && dynamic_cast<CEditStyleBool*>(m_Template->GetEditStyle()));

	CEditStyleBool *style = static_cast<CEditStyleBool*>(this->m_Template->GetEditStyle());

	this->SetCheck(style->strTrue == szVal);
}

LPCTSTR CGridCellCheck::GetText() const
{
	return GetText(GetCheck());
}

LPCTSTR CGridCellCheck::GetText(bool checkstate) const
{
	ASSERT(m_Template && dynamic_cast<CEditStyleBool*>(m_Template->GetEditStyle()));

	CEditStyleBool *style = static_cast<CEditStyleBool*>(this->m_Template->GetEditStyle());
	return (checkstate ? style->strTrue.c_str() : style->strFalse.c_str());
}

CSize CGridCellCheck::GetCellExtent(CDC* pDC)
{
    // Using SM_CXHSCROLL as a guide to the size of the checkbox
    int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();	
    CSize	cellSize = CGridCell::GetCellExtent(pDC);	
    cellSize.cx += nWidth;	
    cellSize.cy = max (cellSize.cy, nWidth);	
    return  cellSize;
}

// i/o:  i=dims of cell rect; o=dims of text rect
BOOL CGridCellCheck::GetTextRect( LPRECT pRect)
{
    BOOL bResult = CGridCell::GetTextRect(pRect);
    if (bResult)
    {
        int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();
        pRect->left += nWidth;
        if (pRect->left > pRect->right)
            pRect->left = pRect->right;

		//不显示文本
		pRect->right = pRect->left - 1;
		pRect->bottom = pRect->top - 1;
    }
    return bResult;
}

CSize CGridCellCheck::GetTextExtent(LPCTSTR szText, CDC* pDC /*= NULL*/)
{
	return CSize(0,0);
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellCheck::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

#ifndef _WIN32_WCE
    // Store the cell's dimensions for later
    m_Rect = rect;

    CRect CheckRect = GetCheckPlacement();
    rect.left = CheckRect.right;

    // enough room to draw?
    // if (CheckRect.Width() < rect.Width() && CheckRect.Height() < rect.Height()) {

    // Do the draw 
    pDC->DrawFrameControl(GetCheckPlacement(), DFC_BUTTON, 
	    (m_bChecked)? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);

    // }
#endif
    return bResult;
}

void CGridCellCheck::OnClick(int nRow, int nCol, CPoint PointCellRelative)
{
	// PointCellRelative is relative to the topleft of the cell. Convert to client coords
	PointCellRelative += m_Rect.TopLeft();

    // Bail if cell is read-only
    CCellID cell = GetGrid()->GetCellFromPt(PointCellRelative);	
    if (!GetGrid()->IsCellEditable(cell))		
        return;

	//TRACE("Check Changeable !\n");

	// GetCheckPlacement returns the checkbox dimensions in client coords. Only check/
	// uncheck if the user clicked in the box
	if (GetCheckPlacement().PtInRect(PointCellRelative))
	{
		//TRACE("Check Changed!\n");
		//m_bChecked = !m_bChecked; // 通过向Grid发送消息来改变内容

		// 为了发送WM_NOTIFY消息，必须创建一个ID为IDC_INPLACE_CONTROL的Grid子窗口
		CWnd wnd; RECT r={1,1,1,1};
		wnd.Create(NULL, TEXT("temp"), SW_HIDE, r, GetGrid(), IDC_INPLACE_CONTROL, NULL);

		// Send Notification to parent
		GV_DISPINFO dispinfo;

		dispinfo.hdr.hwndFrom = wnd.GetSafeHwnd();
		dispinfo.hdr.idFrom   = IDC_INPLACE_CONTROL;
		dispinfo.hdr.code     = GVN_ENDLABELEDIT;

		dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
		dispinfo.item.row     = nRow;
		dispinfo.item.col     = nCol;
		dispinfo.item.strText  = GetText(!m_bChecked);
		dispinfo.item.lParam  = (LPARAM) 0;

		GetGrid()->SendMessage(WM_NOTIFY, IDC_INPLACE_CONTROL, (LPARAM)&dispinfo );
		GetGrid()->InvalidateRect(m_Rect);
	}
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

BOOL CGridCellCheck::SetCheck(BOOL bChecked /*=TRUE*/)
{
	BOOL bTemp = m_bChecked;
	m_bChecked = bChecked;
	if (!m_Rect.IsRectEmpty())
		GetGrid()->InvalidateRect(m_Rect);

	return bTemp;
}

BOOL CGridCellCheck::GetCheck() const
{
	return m_bChecked;
}

//////////////////////////////////////////////////////////////////////
// Protected implementation
//////////////////////////////////////////////////////////////////////

// Returns the dimensions and placement of the checkbox in client coords.
CRect CGridCellCheck::GetCheckPlacement()
{
	int nWidth = GetSystemMetrics(SM_CXHSCROLL);
	CRect place = m_Rect + CSize(GetMargin(), GetMargin());
    place.right = place.left + nWidth;
    place.bottom = place.top + nWidth;

	/* for centering
	int nDiff = (place.Width() - nWidth)/2;
	if (nDiff > 0)
	{
		place.left += nDiff;
		place.right = place.left + nWidth;
	}
	nDiff = (place.Height() - nWidth)/2;
	if (nDiff > 0)
	{
		place.top += nDiff;
		place.bottom = place.top + nWidth;
	}
    */

    if (m_Rect.Height() < nWidth + 2 * static_cast<int> (GetMargin())) 
    {		
        place.top = m_Rect.top + (m_Rect.Height() - nWidth) / 2;	    
        place.bottom = place.top + nWidth;	
    }

	return place;
}