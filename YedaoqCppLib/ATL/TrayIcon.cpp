#include "StdAfx.h"
#include "TrayIcon.h"
#include <atlstdthunk.h>
#include "cpp\FastDelegateBind.h"

#define SAFE_DELETE(x) { delete x; x = 0; }

#define CHECKDELAYCREATE if(!DelayCreate()) return;

CDynamicIcon::CDynamicIcon( DWORD _miniseconds_per_icon, DWORD _miniseconds_life, WORD _icon_count, ... )
	: miniseconds_per_icon(_miniseconds_per_icon), miniseconds_life(_miniseconds_life), icon_count(_icon_count)
{
	ATLASSERT(icon_count > 0);
	icon_handles = new HICON[icon_count];

	HICON* valist = (HICON*)(&_icon_count + 1);
	memcpy(icon_handles, valist, sizeof(HICON) * icon_count);
}

CDynamicIcon::~CDynamicIcon()
{
	HICON* tmp = icon_handles;
	if(icon_handles)
	{
		icon_handles = 0;
		for(int i = 0; i < icon_count; ++i)
		{
			DeleteObject(tmp[i]);
		}
		delete[] tmp;
	}
}

CTrayIcon::CTrayIcon( HWND hCallbackWnd, UINT uCallbackMessage, UINT uID, HICON hIcon, LPCTSTR lpszTip )
	: dynamic_icon_(0), dynamic_timer_(0, 0, RunDynamicIcon, this, false)
{
	memset(&data_, 0, sizeof(NOTIFYICONDATA));
	data_.hWnd = hCallbackWnd;
	data_.uCallbackMessage = uCallbackMessage;
	data_.uID = uID;
	data_.hIcon = hIcon;

	if(lpszTip && lpszTip[0])
	{
		_tcscpy_s(data_.szTip, lpszTip);
	}
}

CTrayIcon::~CTrayIcon(void)
{
	Show(false);
}

void CTrayIcon::SetTip( LPCTSTR lpszTip )
{
	CHECKDELAYCREATE;

	if(lpszTip && lpszTip[0])
	{
		_tcscpy_s(data_.szTip, lpszTip);
	}
	else
	{
		data_.szTip[0] = 0;
	}

	data_.uFlags = NIF_TIP;
	Shell_NotifyIcon(NIM_MODIFY, &data_);
}

void CTrayIcon::InnerSetIcon( HICON icon )
{
	CHECKDELAYCREATE;
	data_.uFlags = NIF_ICON;
	data_.hIcon = icon;
	Shell_NotifyIcon(NIM_MODIFY, &data_);
}

void CTrayIcon::SetIcon( HICON icon )
{
	if(dynamic_icon_)
	{
		SAFE_DELETE(dynamic_icon_);
	}
	InnerSetIcon(icon);
}

void CTrayIcon::SetIcon( CDynamicIcon* icon )
{
	
}

void CTrayIcon::Show( bool fShow )
{
	if(fShow)
	{
		CHECKDELAYCREATE;
	}
	else
	{
		data_.uFlags = 0;
		BOOL bRet = Shell_NotifyIcon(NIM_DELETE, &data_);
		data_.cbSize = 0;
	}
}

void CTrayIcon::ShowBalloonInfo( LPCTSTR lpszInfo, LPCTSTR lpszTitle, EnumBalloonIcon system_icon, HICON custom_icon, EnumBalloonFlag flag )
{
	if(lpszInfo && lpszInfo[0])
	{
		_tcscpy_s(data_.szInfo, lpszInfo);
	}
	else
	{
		data_.szInfo[0] = 0;
	}

	if(lpszTitle && lpszTitle[0])
	{
		_tcscpy_s(data_.szInfoTitle, lpszTitle);
	}
	else
	{
		data_.szInfoTitle[0] = 0;
	}

	HICON tmp = data_.hIcon;

	if(NULL != custom_icon)
	{
		data_.dwInfoFlags = NIIF_USER;
		data_.hIcon = custom_icon;
	}
	data_.dwInfoFlags |= flag;
	data_.uFlags = NIF_INFO;

	BOOL bRet = Shell_NotifyIcon(NIM_MODIFY, &data_);
	data_.hIcon = tmp;
}

bool CTrayIcon::DelayCreate()
{
	if(0xFFFF == data_.cbSize) return false;
	if(data_.cbSize > 0) return true;

	//Make sure Notification window is valid
	do 
	{
		data_.cbSize = 0xFFFF;

		if(!::IsWindow(data_.hWnd)) break;

		data_.cbSize = 0;

	} while (false);

	if(0xFFFF == data_.cbSize)
	{
		return false;
	}

	// load up the NOTIFYICONDATA structure
	data_.cbSize = GetAdaptiveSize4IconData();
	data_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	// Set the tray icon
	BOOL bRet = Shell_NotifyIcon(NIM_ADD, &data_);
	if(!bRet)
	{
		data_.cbSize = 0;
	}
	return TRUE == bRet;
}

DWORD CTrayIcon::GetAdaptiveSize4IconData()
{
	OSVERSIONINFOEX osvi;
	memset(&osvi, 0, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if( !GetVersionEx ((OSVERSIONINFO *) &osvi))
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return 0;
	}

	if(osvi.dwMajorVersion >= 6)
	{
		return sizeof(TrayIconData);
	}
	else if(osvi.dwMajorVersion == 5)
	{
		if(osvi.dwMinorVersion >= 1)
			return NOTIFYICONDATA_V3_SIZE;
		else
			return NOTIFYICONDATA_V2_SIZE;
	}
	else
	{
		return NOTIFYICONDATA_V1_SIZE;
	}
}

bool CTrayIcon::RunDynamicIcon( CTimerEx* timer, int flag )
{

}

