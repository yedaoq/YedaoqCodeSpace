/* ___________________________________________________________________________
@ 
@ file - OcxFile.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-2-7
@ info -
@     用于辅助创建ActiveX控件，可创建未注册组件
@	  指定路径时，作为未注册组件来创建组件，否则作为已注册组件创建
@	  CAxWindow可根据ProgID，CLSID和Name来创建组件，但本类仅支持CISID
/* ___________________________________________________________________________*/

#pragma once
#include "ComFile.h"
#include "CAtlComModuleScope.h"

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
	CAtlComModuleScope AtlAxWinInitScope;
};
