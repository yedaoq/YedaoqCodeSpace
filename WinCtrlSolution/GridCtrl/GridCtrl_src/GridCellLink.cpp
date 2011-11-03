// GridURLCell.cpp: implementation of the CGridURLCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCellLink.h"
#include "CellTemplate.h"
#include "GridCtrl.h"


IMPLEMENT_DYNCREATE(CGridCellLink, CGridCell)

#ifndef _WIN32_WCE
HCURSOR CGridCellLink::g_hLinkCursor = NULL;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridCellLink::CGridCellLink()
{
#ifndef _WIN32_WCE
    g_hLinkCursor = GetHandCursor();
#endif
	m_bLaunchUrl = TRUE;
	m_clrUrl = GetSysColor(COLOR_HIGHLIGHT);
}

CGridCellLink::~CGridCellLink()
{
}

BOOL CGridCellLink::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd)
{
	// If url is present then change text color
	//if (HasUrl(GetText()))
		SetTextClr(m_clrUrl);

    // Good a place as any to store the bounds of the rect
    m_Rect = rect;

    return CGridCell::Draw(pDC, nRow, nCol, rect, bEraseBkgnd);
}

#pragma warning(disable:4100)
BOOL CGridCellLink::Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar)
{
    return FALSE;
}
#pragma warning(default:4100)

void CGridCellLink::OnClick(int nRow, int nCol, CPoint PointCellRelative)
{
#ifndef _WIN32_WCE
    CString strURL;
    if (/*GetAutoLaunchUrl() &&*/ OverURL(PointCellRelative, strURL))
	{
		ASSERT(m_Template && m_Template->GetEditStyle());
		CEditStyleLink * iInput = static_cast<CEditStyleLink*>(GetTemplate()->GetEditStyle());
		CGridCellLinkInputContext ctx(GetText(), nRow, nCol);
		tstring str = iInput->OnLinkClicked(&ctx);

		GV_DISPINFO dispinfo;

		dispinfo.hdr.hwndFrom = NULL;
		dispinfo.hdr.idFrom   = 0;
		dispinfo.hdr.code     = GVN_ENDLABELEDIT;
		dispinfo.item.row     = nRow;
		dispinfo.item.col     = nCol;
		dispinfo.item.strText  = str.c_str();

		CGridCtrl* pGrid = GetGrid();
		if(pGrid)
		{
			pGrid->SendMessage(WM_NOTIFY, 0, (LPARAM)&dispinfo );
		}
		
		throw std::exception();
	}
#endif
}

// Return TRUE if you set the cursor
BOOL CGridCellLink::OnSetCursor(int nRow, int nCol)
{
#ifndef _WIN32_WCE
    CString strURL;
    CPoint pt(GetMessagePos());
    GetGrid()->ScreenToClient(&pt);
    pt = pt - m_Rect.TopLeft();

    if (OverURL(pt, strURL))
    {
        SetCursor(g_hLinkCursor);
		return TRUE;
	}
	else
#endif
		return CGridCell::OnSetCursor(nRow, nCol);
}

#ifndef _WIN32_WCE
HCURSOR CGridCellLink::GetHandCursor()
{
	if (g_hLinkCursor == NULL)		// No cursor handle - load our own
	{
        // Get the windows directory
		CString strWndDir;
		GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
		strWndDir.ReleaseBuffer();

		strWndDir += _T("\\winhlp32.exe");
		// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
		HMODULE hModule = LoadLibrary(strWndDir);
		if( hModule )
		{
			HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
			if( hHandCursor )
			{
				g_hLinkCursor = CopyCursor(hHandCursor);
			}
		}
		FreeLibrary(hModule);
	}

	return g_hLinkCursor;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions

BOOL CGridCellLink::HasUrl(CString str)
{
    return TRUE;
}

// here we figure out if we are over a URL or not
BOOL CGridCellLink::OverURL(CPoint& pt, CString& strURL)
{
    //TRACE2("Checking point %d,%d\n",pt.x,pt.y);

	BOOL bOverURL = FALSE;
	CSize size = GetTextExtent(GetText());

	// Add left of cell so we know if we clicked on text or not
	pt.x += m_Rect.left;
	CPoint center = m_Rect.CenterPoint();

	if ((m_nTextDrawFormat & DT_RIGHT) && pt.x >= (m_Rect.right - size.cx))
	{
		bOverURL = TRUE;
	}	
	else if ((m_nTextDrawFormat & DT_CENTER) && 
             ((center.x - (size.cx/2)) <= pt.x) && (pt.x <= (center.x + (size.cx/2))) )
	{
		bOverURL = TRUE;
	}
	else if (pt.x <= (size.cx + m_Rect.left))
	{
		bOverURL = TRUE;
	}

    if (!bOverURL)
        return FALSE;

    // We are over text - but are we over a URL?
	bOverURL = FALSE;
	strURL = GetText();

	// Use float, otherwise we get an incorrect letter from the point
	float width = (float)size.cx/(float)strURL.GetLength();

	// remove left of cell so we have original point again 
	pt.x -= m_Rect.left;
	if (m_nTextDrawFormat & DT_RIGHT)
	{
		int wide = m_Rect.Width() - size.cx;
		pt.x -= wide;
		if (pt.x <= 0)
			return FALSE;
	}

	if (m_nTextDrawFormat & DT_CENTER)
	{
		int wide = m_Rect.Width() - size.cx;
		pt.x -= (wide/2);
		if (pt.x <= 0 || pt.x > (size.cx + (wide/2)))
			return FALSE;
	}

	// Turn point into a letter
	int ltrs = (int)((float)pt.x/width);
#if  !defined(_WIN32_WCE) || (_WIN32_WCE > 210)
	// Find spaces before and after letter, process text between
	int endSpace = strURL.Find(_T(' '), ltrs);
	if (endSpace != -1)
		strURL.Delete(endSpace, strURL.GetLength()-endSpace);

	int beginSpace = strURL.ReverseFind(_T(' '));
	if (beginSpace != -1)
		strURL.Delete(0, ++beginSpace);
#endif

	// Does text have url
	return HasUrl(strURL);
}

