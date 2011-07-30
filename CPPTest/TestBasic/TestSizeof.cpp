#include "stdafx.h"
#include<stdio.h>
#include <iostream>

using namespace std;

void fun(const int* buf)
{
	cout<<"Size of buf : "<<sizeof(buf)<<endl;
}

int mainSizeof()
{	
	int iArray[10];
	int *pArray = new int[10];

	cout<<"Size of iArray : "<<sizeof(iArray)<<endl;
	cout<<"Size of pArray : "<<sizeof(pArray)<<endl;

	fun(iArray);
	fun(pArray);
	
	getchar();
	
	return 1;
}
