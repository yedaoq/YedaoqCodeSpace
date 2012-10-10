#ifndef YedaoqCppLib_Win32_TrayIcon_h__
#define YedaoqCppLib_Win32_TrayIcon_h__

#include <shellapi.h>
#include "TimerEx.h"

#ifndef NIIF_LARGE_ICON
#define NIIF_LARGE_ICON 0x00000020
#endif

struct CDynamicIconRunInfo;
interface ITrayIconCallBack;

enum EnumBalloonIcon {TIBI_INFO = NIIF_INFO, TIBI_WARNING = NIIF_WARNING, TIBI_ERROR = NIIF_ERROR };
enum EnumBalloonFlag {TIBF_NONE = 0, TIBF_NOSOUND = NIIF_NOSOUND, TIBF_LARGE_ICON = NIIF_LARGE_ICON };

struct CDynamicIcon
{
	CDynamicIcon(DWORD _miniseconds_per_icon, DWORD _miniseconds_life, WORD _icon_count, ...);
	~CDynamicIcon();

	DWORD		miniseconds_per_icon;
	DWORD		miniseconds_life;
	DWORD		icon_count;
	HICON*		icon_handles;
};

class CTrayIcon
{
	struct TrayIconData
	{
		DWORD	cbSize;
		HWND	hWnd;
		UINT	uID;
		UINT	uFlags;
		UINT	uCallbackMessage;
		HICON	hIcon;
		TCHAR	szTip[128];
		DWORD	dwState;
		DWORD	dwStateMask;
		TCHAR	szInfo[256];
		union {
			UINT  uTimeout;
			UINT  uVersion;  // used with NIM_SETVERSION, values 0, 3 and 4
		} DUMMYUNIONNAME;
		TCHAR	szInfoTitle[64];
		DWORD	dwInfoFlags;
		GUID	guidItem;
		HICON	hBalloonIcon;
	};

public:
	CTrayIcon(HWND hCallbackWnd, UINT uCallbackMessage, UINT uID, HICON hIcon, LPCTSTR lpszTip);
	~CTrayIcon(void);

	void SetTip(LPCTSTR lpszTip);
	void SetIcon(HICON icon);
	void SetIcon(CDynamicIcon* icon);
	
	void Show(bool fShow);
	void ShowBalloonInfo(LPCTSTR lpszInfo, LPCTSTR lpszTitle, EnumBalloonIcon system_icon, HICON custom_icon, EnumBalloonFlag flag);

protected:
	bool DelayCreate();
	void InnerSetIcon(HICON icon);

	static DWORD GetAdaptiveSize4IconData();
	static bool __stdcall RunDynamicIcon(CTimerEx* timer, int flag);

protected:
	NOTIFYICONDATA			data_;
	CDynamicIcon*			dynamic_icon_;
	CTimerEx				dynamic_timer_;
	HICON					backup_icon_;
};

#endif // YedaoqCppLib_Win32_TrayIcon_h__

