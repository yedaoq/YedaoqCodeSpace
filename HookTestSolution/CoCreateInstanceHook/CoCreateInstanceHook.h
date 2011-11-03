#pragma once

extern "C" HHOOK*		WINAPI GetHookHandle();
extern "C" HMODULE	WINAPI GetHookModule();

extern "C" LRESULT CALLBACK  GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);

extern "C" LRESULT	WINAPI	   Clear();
