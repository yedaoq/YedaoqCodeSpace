// TestPackAlignment.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

struct S1
{
	char c;
	long l;
};

struct S2
{
	char c;
	short s;
};

struct S3
{
	char c;

	S1 s;

	long l;
};

#pragma pack(1)

struct S4
{
	char c;
	long l;
};



int main(int argc, char* argv[])
{
	S1 s1;
	S2 s2;
	S3 s3;
	S4 s4;

	cout<<"Char  - S1.c : "<<(void *)(&s1.c)<<endl;
	cout<<"Long  - S1.l : "<<(void *)(&s1.l)<<endl<<endl;

	cout<<"Char  - S2.c : "<<(void *)(&s2.c)<<endl;
	cout<<"Short - S2.l : "<<(void *)(&s2.s)<<endl<<endl;

	cout<<"Char  - S3.c : "<<(void *)(&s3.c)<<endl;
	cout<<"S1    - S3.s : "<<(void *)(&s3.s)<<endl;
	cout<<"Short - S3.l : "<<(void *)(&s3.l)<<endl<<endl;

	cout<<"Char  - S4.c : "<<(void *)(&s4.c)<<endl;
	cout<<"Long  - S4.l : "<<(void *)(&s4.l)<<endl<<endl;

	//scanf(s1.c);

	cin>>s1.c;
	
	return 0;
}

