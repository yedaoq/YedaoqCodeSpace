// CoCreateInstanceHook.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "detours\detours.h"
#include <winbase.h>
#include <streams.h>

bool	g_CoCreateInstanceHookFlag = false;
HHOOK	g_hHook = INVALID_HANDLE_VALUE;

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
		DetourHookCoCreateInstance();
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

void DetourHookCoCreateInstance()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)CoCreateInstancePtr, (PVOID&)CoCreateInstanceMine);
	DetourTransactionCommit();
}

HRESULT WINAPI CoCreateInstanceMine(__in     REFCLSID rclsid, 
									__in_opt LPUNKNOWN pUnkOuter,
									__in     DWORD dwClsContext, 
									__in     REFIID riid, 
									__deref_out LPVOID FAR* ppv)
{
	if(CLSID_FilterGraph == rclsid)
	{
		HRESULT hr = CoCreateInstancePtr(rclsid, pUnkOuter, dwClsContext, riid, ppv);
		if(SUCCEEDED(hr))
		{
			DWORD dwRegister;
			AddToRot(*(IUnknown**)ppv, &dwRegister);
		}
	}
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