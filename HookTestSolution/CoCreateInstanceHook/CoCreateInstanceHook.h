#pragma once

extern "C" HHOOK* WINAPI	GetHookHandle();
extern "C" LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);

