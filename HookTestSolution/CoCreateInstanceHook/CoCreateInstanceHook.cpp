// CoCreateInstanceHook.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "detours\detours.h"
#include <winbase.h>
#include <uuids.h>
#include "DebugLog.h"
#include <Helper.h>

#pragma comment(lib, "Strmiids")

bool	g_CoCreateInstanceHookFlag = false;
HHOOK	g_hHook = NULL;

void DetourHookCoCreateInstance();
HRESULT AddToRot(IUnknown *pUnkGraph,DWORD *pdwRegister);

HRESULT (WINAPI *CoCreateInstancePtr)(__in     REFCLSID rclsid, 
									  __in_opt LPUNKNOWN pUnkOuter,
									  __in     DWORD dwClsContext, 
									  __in     REFIID riid, 
									  __deref_out LPVOID FAR* ppv)
									  = CoCreateInstance;

HRESULT WINAPI CoCreateInstanceMine(__in     REFCLSID rclsid, 
									__in_opt LPUNKNOWN pUnkOuter,
									__in     DWORD dwClsContext, 
									__in     REFIID riid, 
									__deref_out LPVOID FAR* ppv) throw( );




HHOOK* WINAPI GetHookHandle()
{
	return &g_hHook;
}



LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(!g_CoCreateInstanceHookFlag)
	{
		MessageBox(0, TEXT("hood get"), TEXT("hook"), MB_OK);
		DetourHookCoCreateInstance();
		UnhookWindowsHook(WH_GETMESSAGE, GetMsgProc);
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

void DetourHookCoCreateInstance()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)CoCreateInstancePtr, (PVOID&)CoCreateInstanceMine);
	DetourTransactionCommit();
	WTRACE(TEXT("Detour Hook CoCreateInstance\n"));
}

LRESULT WINAPI	Clear()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)CoCreateInstancePtr, (PVOID&)CoCreateInstanceMine);
	DetourTransactionCommit();

	WTRACE(TEXT("Detour Unhook CoCreateInstance\n"));

	return 0;
}

HRESULT WINAPI CoCreateInstanceMine(__in     REFCLSID rclsid, 
									__in_opt LPUNKNOWN pUnkOuter,
									__in     DWORD dwClsContext, 
									__in     REFIID riid, 
									__deref_out LPVOID FAR* ppv)
{
	HRESULT hr;
	hr = CoCreateInstancePtr(rclsid, pUnkOuter, dwClsContext, riid, ppv);

	Log(TEXT("Get Call to CoCreateInstance "));
	if(SUCCEEDED(hr) && CLSID_FilterGraph == rclsid)
	{
		Log(TEXT("of CLSID_FilterGraph"));
		DWORD dwRegister;
		AddToRot(*(IUnknown**)ppv, &dwRegister);
	}

	Log(TEXT("\n"));

	return hr;
}

HRESULT AddToRot(IUnknown *pUnkGraph,DWORD *pdwRegister)
{
	IMoniker				*pMoniker;
	IRunningObjectTable		*pROT;
	if(FAILED(GetRunningObjectTable(0, &pROT)))
	{
		return E_FAIL;
	}
	WCHAR   wsz[256];

	wsprintfW(wsz, L"FilterGraph   %08p   pid   %08x ", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
	HRESULT hr = CreateItemMoniker(L"! ", wsz, &pMoniker);

	if(SUCCEEDED(hr))  
	{
		hr = pROT->Register(0,pUnkGraph, pMoniker, pdwRegister);
		pMoniker->Release();
	}
	pROT->Release();
	return   hr;
} 