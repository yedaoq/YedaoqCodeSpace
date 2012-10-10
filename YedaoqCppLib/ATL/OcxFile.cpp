#include "StdAfx.h"
#include "OcxFile.h"
#include "microtype.h"

HRESULT COcxFile::CreateControl( ATL::CAxWindow& target, HWND hParent, ATL::_U_RECT rect /*= NULL */, DWORD dwStyle /*= 0*/, DWORD dwExStyle /*= 0*/, ATL::_U_MENUorID MenuOrID /*= 0U*/, LPVOID lpCreateParam /*= NULL*/ )
{
	if(FileName_.empty())
	{
		return CreateRegisteredControl(target, hParent, rect, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
	}
	else
	{
		return CreateUnregisteredControl(target, hParent, rect, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
	}
}

HRESULT COcxFile::CreateUnregisteredControl( ATL::CAxWindow& target, HWND hParent, ATL::_U_RECT rect /*= NULL */, DWORD dwStyle /*= 0*/, DWORD dwExStyle /*= 0*/, ATL::_U_MENUorID MenuOrID /*= 0U*/, LPVOID lpCreateParam /*= NULL*/ )
{
	HRESULT				hr;
	CComPtr<IUnknown>	pUnknow;
	HWND				hWnd = target;

	SINGLELOOP
	{
		hr = CreateInstance(__uuidof(IUnknown), (void**)&pUnknow, NULL);
		if(!SUCCEEDED(hr))
		{
			break;
		}

		if(NULL == hWnd)
		{
			hWnd = ::CreateWindowEx(dwExStyle, TEXT("STATIC"), NULL, SS_GRAYRECT
				, rect.m_lpRect->left, rect.m_lpRect->top, rect.m_lpRect->right - rect.m_lpRect->left, rect.m_lpRect->bottom - rect.m_lpRect->top
				, hParent, MenuOrID.m_hMenu, NULL, lpCreateParam);

			if(NULL == hWnd)
			{
				hr = E_FAIL;
				break;
			}

			target = hWnd;
		}

		hr = target.AttachControl(pUnknow, NULL);
	}

	return hr;
}

HRESULT COcxFile::CreateRegisteredControl( ATL::CAxWindow& target, HWND hParent, ATL::_U_RECT rect /*= NULL */, DWORD dwStyle /*= 0*/, DWORD dwExStyle /*= 0*/, ATL::_U_MENUorID MenuOrID /*= 0U*/, LPVOID lpCreateParam /*= NULL*/ )
{
	WCHAR pBuf[48];
	::StringFromGUID2(Clsid_, pBuf, 48);
	return (NULL != target.Create(hParent, rect, pBuf, dwStyle, dwExStyle, MenuOrID, lpCreateParam))
		? S_OK : E_FAIL;
}

