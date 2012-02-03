#pragma once
#include "ComFile.h"
#include <atlwin.h>

class CAtlAxWinInitScope
{
protected:
	static unsigned int RefCount;

public:
	CAtlAxWinInitScope();
	~CAtlAxWinInitScope();
};

class COcxFile : public CComFile
{
public:
	COcxFile(REFCLSID clsid, LPCTSTR lpOcxPath = 0, LPCTSTR lpLicence = 0)
		: CComFile(clsid, lpOcxPath, lpLicence)
	{}

protected:
	COcxFile& operator=(const COcxFile&);

public:

	HRESULT CreateControl(ATL::CAxWindow& pTarget, HWND hParent, ATL::_U_RECT rect = NULL
		, DWORD dwStyle = 0, DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);

	HRESULT CreateUnregisteredControl(ATL::CAxWindow& pTarget, HWND hParent, ATL::_U_RECT rect = NULL
		, DWORD dwStyle = 0, DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);

	HRESULT CreateRegisteredControl(ATL::CAxWindow& pTarget, HWND hParent, ATL::_U_RECT rect = NULL
		, DWORD dwStyle = 0, DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);

protected:
	CAtlAxWinInitScope AtlAxWinInitScope;
};
