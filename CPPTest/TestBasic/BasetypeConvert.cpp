#include "stdafx.h"
#include<stdio.h>
#include <iostream>

using namespace std;

int mainBasetypeConvert()
{
    int i = -1;
	unsigned int a = i;
	char c = i;
	char c1= 0xaf;
	signed char c2 = 0xaf;
	
	std::cout<<i<<"    "<<a<<"   |"<<(int)c<<"|   |"<<(int)c1 <<"|   |"<<(int)c2 <<endl;

	getchar();

	return 1;
}
