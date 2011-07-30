// TestBasic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <winbase.h>
#include <string.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <mbstring.h>

#include "test.h"

using namespace std;

void test1()
{
	printf("Hello World!\n");
	char buffer[10] = "我是abc";
	printf("%s len : %d\r\n",buffer,strlen(buffer));
	printf("%s len : %d\r\n",buffer,_mbslen((const unsigned char *)buffer));

	// Conclusion : 字面量字符串将按多字节字符集编码，若带L前缀，则将编译为Unicode字符串。
}

void test2()
{
	int i = 5;

	char *str = (char*)(&i);

	cout<< "(str = i) : "<< str << endl;

	char c = i;
	
	cout<< "(c = i) : "<< c << endl;

	c = (char)i;
	
	cout<< "(c = (char)i) : "<< c << endl;

	c = static_cast<char>(i);

	cout<< "(c = static_cast<char>(i)) : "<< c << endl;

	i = c;

	cout<< "(i = c) : " << i <<endl;

	i = static_cast<int>(i);
	
	cout<< "(i = static_cast<int>(c)) : "<< i << endl;

	// Conclusion : Intel 字节序为 LITTLE-ENDIAN
	// Conclusion : static_cast 的功能与 强制转换 相同，但static_cast只能用于同一个派系的类型间转换
	// Conclusion : 不存在语义层次的强制转换，那需要通过atoi等函数现实现
}

int main1(int argc, char* argv[])
{
	test3();

	return 0;
}

