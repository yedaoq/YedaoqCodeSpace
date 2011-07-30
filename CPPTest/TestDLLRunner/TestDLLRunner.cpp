// TestDLLRunner.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

extern "C" __declspec(dllimport) VOID __stdcall TipPath();
#pragma comment(lib,"TestDLL.lib")

int main(int argc, char* argv[])
{
	TipPath();
	return 0;
}

