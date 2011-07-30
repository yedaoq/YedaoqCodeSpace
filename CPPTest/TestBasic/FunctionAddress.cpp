#include "stdafx.h"
#include<stdio.h>
#include <iostream>

using namespace std;

void TestFunctionAddress()
{

}

typedef void (*pFunVoid)();

int mainFunctionAddress()
{	
	pFunVoid pf = TestFunctionAddress;
	pFunVoid pf1 = &TestFunctionAddress;

	getchar();
	return 0;
}
