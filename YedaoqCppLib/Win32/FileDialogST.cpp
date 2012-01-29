#include "stdafx.h"
#include "FileDialogST.h"
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// Constructs a CFileDialogST object.
// Most frequently used parameters can be passed on the argument list.
//
// Parameters:
//		[IN]	bOpenFileDialog
//				Set to TRUE to construct a File Open dialog box or
//				FALSE to construct a File Save As dialog box.
//		[IN]	lpszDefExt
//				The default filename extension.
//				If the user does not include an extension in the Filename edit box,
//				the extension specified by lpszDefExt is automatically appended
//				to the filename.
//				If this parameter is NULL, no file extension is appended.
//		[IN]	lpszFileName
//				The initial filename that appears in the filename edit box.
//				If NULL, no filename initially appears
//		[IN]	dwFlags
//				A combination of one or more flags that allow you to customize the dialog box.
//		[IN]	lpszFilter
//				A series of string pairs that specify filters you can apply to the file.
//				If you specify file filters, only selected files will appear in the
//				Files list box.
//		[IN]	pParentWnd
//				Pointer to the owner window for the dialog box. Can be NULL.
//
CFileDialogST::CFileDialogST(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, HWND pParentWnd)
{
	::ZeroMemory(&m_ofn, sizeof(m_ofn));
	::ZeroMemory(&m_szFile, sizeof(m_szFile));
	::ZeroMemory(&m_szFileTitle, sizeof(m_szFileTitle));
	::ZeroMemory(&m_szSelectedFolder, sizeof(m_szSelectedFolder));

	// Store parameters
	m_bOpenFileDialog = bOpenFileDialog;

	DWORD dwWinMajor = (DWORD)(LOBYTE(LOWORD(::GetVersion())));
	m_ofn.lStructSize = sizeof(OPENFILENAME);

	m_ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;// dwFlags;// | OFN_EXPLORER;

	m_ofn.lpstrFile = m_szFile;
	m_ofn.nMaxFile = MAX_PATH;

	m_ofn.lpstrFileTitle = m_szFileTitle;
	m_ofn.nMaxFileTitle = MAX_PATH;

	if (lpszFilter != NULL)
	{
		_tcscpy_s(m_szFilter, sizeof(m_szFilter)/sizeof(TCHAR), lpszFilter);
		TCHAR* pChar = m_szFilter;
		while ((pChar = _tcschr(pChar, '|')) != NULL)
			*pChar++ = '\0';
		m_ofn.lpstrFilter = m_szFilter;
		m_ofn.nFilterIndex = 1;
	}

	m_ofn.hwndOwner = pParentWnd;
	m_ofn.lpstrDefExt = lpszDefExt;

	if(lpszFileName)
	{
		_tcscpy_s(m_szFile, lpszFileName);
	}
}

CFileDialogST::CFileDialogST()
{
	::ZeroMemory(&m_ofn, sizeof(m_ofn));
	::ZeroMemory(&m_szFile, sizeof(m_szFile));
	::ZeroMemory(&m_szFileTitle, sizeof(m_szFileTitle));
	::ZeroMemory(&m_szSelectedFolder, sizeof(m_szSelectedFolder));
	m_bOpenFileDialog = TRUE;
}

// Constructs a CFileDialogST object.
// All required parameters must be initialized by hand accessing
// the m_ofn and m_bOpenFileDialog public members.
CFileDialogST::~CFileDialogST()
{
}

// This function displays the file selection dialog box and allows the user to make a selection.
// All required fields of the m_ofn public structure must be filled. This can be done
// using the class constructor or accessing directly the structure. Also the public
// variable m_bOpenFileDialog must be set to TRUE to get an open dialog box or to FALSE to
// get a save dialog box.
//
// Return value:
//		IDOK
//			The user has selected a filename.
//		IDCANCEL
//			The user has closed the dialog without selecting any filename.
//
int CFileDialogST::DoModal()
{
	BOOL	bRetValue;

	// Execute dialog
	if (m_bOpenFileDialog)
		bRetValue = ::GetOpenFileName(&m_ofn);
	else
		bRetValue = ::GetSaveFileName(&m_ofn);

	return (bRetValue ? IDOK : IDCANCEL);
} // End of DoModal

// This function returns the full path of the selected file.
//
// Return value:
//			A LPCTSTR object containing the full path of the file.
//
LPCTSTR CFileDialogST::GetPathName() const
{
	return m_ofn.lpstrFile;
} // End of GetPathName

// This function returns the filename of the selected file.
//
// Return value:
//			A LPCTSTR object containing the name of the file.
//
LPCTSTR CFileDialogST::GetFileName() const
{
	return m_ofn.lpstrFileTitle;
} // End of GetFileName


// This function returns the full path of the next selected file.
//
// Parameters:
//		[IN]	pos
//				A reference to a POSITION value returned by a previous GetNextPathName 
//				or GetStartPosition function call. 
//				NULL if the end of the list has been reached.
//
// Return value:
//			A LPCTSTR object containing the full path of the file.
//
// Note:	this function has been copied exactly from the MFC
//			implementation of the CFileDialog class.
//
//LPCTSTR CFileDialogST::GetNextPathName(LPCTSTR* cur) const
//{
//	BOOL bExplorer = m_ofn.Flags & OFN_EXPLORER;
//	TCHAR chDelimiter;
//	if (bExplorer)
//		chDelimiter = '\0';
//	else
//		chDelimiter = ' ';
//
//	if(!cur)
//	{
//		return m_ofn.lpstrFile;
//	}
//
//
//
//	LPTSTR lpsz = (LPTSTR)pos;
//	if (lpsz == m_ofn.lpstrFile) // first time
//	{
//		if ((m_ofn.Flags & OFN_ALLOWMULTISELECT) == 0)
//		{
//			pos = NULL;
//			return m_ofn.lpstrFile;
//		}
//
//		// find char pos after first Delimiter
//		while(*lpsz != chDelimiter && *lpsz != '\0')
//			lpsz = _tcsinc(lpsz);
//		lpsz = _tcsinc(lpsz);
//
//		// if single selection then return only selection
//		if (*lpsz == 0)
//		{
//			pos = NULL;
//			return m_ofn.lpstrFile;
//		}
//	}
//
//	LPCTSTR strPath = m_ofn.lpstrFile;
//	if (!bExplorer)
//	{
//		LPTSTR lpszPath = m_ofn.lpstrFile;
//		while(*lpszPath != chDelimiter)
//			lpszPath = _tcsinc(lpszPath);
//		strPath = strPath.Left(lpszPath - m_ofn.lpstrFile);
//	}
//
//	LPTSTR lpszFileName = lpsz;
//	LPCTSTR strFileName = lpsz;
//
//	// find char pos at next Delimiter
//	while(*lpsz != chDelimiter && *lpsz != '\0')
//		lpsz = _tcsinc(lpsz);
//
//	if (!bExplorer && *lpsz == '\0')
//		pos = NULL;
//	else
//	{
//		if (!bExplorer)
//			strFileName = strFileName.Left(lpsz - lpszFileName);
//
//		lpsz = _tcsinc(lpsz);
//		if (*lpsz == '\0') // if double terminated then done
//			pos = NULL;
//		else
//			pos = (POSITION)lpsz;
//	}
//
//	// only add '\\' if it is needed
//	if (!strPath.IsEmpty())
//	{
//		// check for last back-slash or forward slash (handles DBCS)
//		LPCTSTR lpsz = _tcsrchr(strPath, '\\');
//		if (lpsz == NULL)
//			lpsz = _tcsrchr(strPath, '/');
//		// if it is also the last character, then we don't need an extra
//		if (lpsz != NULL &&
//			(lpsz - (LPCTSTR)strPath) == strPath.GetLength()-1)
//		{
//			ASSERT(*lpsz == '\\' || *lpsz == '/');
//			return strPath + strFileName;
//		}
//	}
//	return strPath + '\\' + strFileName;
//} // End of GetNextPathName

int __stdcall CFileDialogST::BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && lpData != NULL)
	{
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	else // uMsg == BFFM_SELCHANGED
	{
	}

	return 0;
} // End of BrowseCtrlCallback

// This function lets the user to selec a folder.
//
// Parameters:
//		[IN]	lpszTitle
//				Address of a null-terminated string that is displayed above the 
//				tree view control in the dialog box. This string can be used to 
//				specify instructions to the user. Can be NULL.
//		[IN]	lpszStartPath
//				Address of a null-terminated string containing the initial folder
//				to open. Can be NULL.
//		[IN]	ulFlags
//				Flags specifying the options for the dialog box.
//		[IN]	pParentWnd
//				Pointer to the owner window for the dialog box. Can be NULL.
//
// Return value:
//		IDOK
//			The user has selected a folder and pressed OK. A call
//			to GetSelectedFolder() will return the selected folder.
//		IDCANCEL
//			The user has closed the dialog without selecting any folder.
//
int CFileDialogST::SelectFolder(LPCTSTR lpszTitle, LPCTSTR lpszStartPath, UINT ulFlags, HWND pParentWnd)
{
	LPMALLOC		pMalloc;
	BROWSEINFO		bi;
	LPITEMIDLIST	pidl;
	int				nRetValue = IDCANCEL;

	::ZeroMemory(&bi, sizeof(bi));

	// Gets the Shell's default allocator
	if (::SHGetMalloc(&pMalloc) == NOERROR)
	{
		// Get help on BROWSEINFO struct - it's got all the bit settings.
		if (pParentWnd != NULL)
			bi.hwndOwner = pParentWnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = m_szSelectedFolder;
		bi.lpszTitle = lpszTitle;
		bi.ulFlags = ulFlags;
		bi.lpfn = BrowseCtrlCallback;
		bi.lParam = (LPARAM)lpszStartPath;
		// This next call issues the dialog box.
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)
		{
			if (::SHGetPathFromIDList(pidl, m_szSelectedFolder))
			{ 
				// At this point pszBuffer contains the selected path
				nRetValue = IDOK;
			} // if
			// Free the PIDL allocated by SHBrowseForFolder.
			pMalloc->Free(pidl);
		} // if
		// Release the shell's allocator.
		pMalloc->Release();
	} // if

	return nRetValue;
} // End of SelectFolder

// This function returns the folder selected by the user with a call to SelectFolder.
//
// Return value:
//			A LPCTSTR object containing the selected folder.
//			Without a previous call to SelectFolder this string can be empty or
//			reflect the last selected folder.
//
LPCTSTR CFileDialogST::GetSelectedFolder() const
{
	return m_szSelectedFolder;
} // End of GetSelectedFolder
