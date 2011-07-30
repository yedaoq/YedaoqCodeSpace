#include "stdafx.h"
#include<stdio.h>
#include <iostream>

using namespace std;

class TypeA
{
	int memA;
	int memB;

	static int count;

public :
	TypeA()
	{
		cout<< "   Constructor execute ... "<<endl;
		memA = ++count;
		memB = count;
	}

	TypeA(TypeA &s)
	{
		cout<< "   Copy Constructor execute ... "<<endl;
		memA = s.memA;
		memB = s.memB;

		++count;
	}

	TypeA& operator=(TypeA &other)
	{
		cout<< "   Assign Operator execute ... "<<endl;

		this->memA = other.memA;
		this->memB = other.memB;

		return *this;
	}

	TypeA operator+(TypeA &a)
	{
		//cout<< "   Add Operator execute ... "<<endl;

		TypeA result;
		result.memA = a.memA + this->memA;
		result.memB = a.memB + this->memB;

		return result;
	}

	static TypeA Create()
	{
		return TypeA();
	}

	static TypeA CreateX()
	{
		TypeA obj;
		return obj;
	}

};

int TypeA::count = 0;

int mainAddAssign()
{
	cout<<"TypeA a;"<<endl;
    TypeA a;

	cout<<endl<<"TypeA b;"<<endl;
	TypeA b;

	cout<<endl<<"TypeA c = a;"<<endl;
	TypeA c = a;

	cout<<endl<<"a + b;"<<endl;
	a + b;

	cout<<endl<<"b = a + b;"<<endl;
	b = a + b;

	cout<<endl<<"TypeA d = a + b;"<<endl;
	TypeA d = a + b;

	cout<<endl<<"TypeA e = TypeA::Create();"<<endl;
	TypeA e = TypeA::Create();

	cout<<endl<<"TypeA f = TypeA::CreateX();"<<endl;
	TypeA f = TypeA::CreateX();

	cout<<endl<<"a = TypeA::Create();"<<endl;
	a = TypeA::Create();

	cout<<endl<<"a = TypeA::CreateX();"<<endl;
	a = TypeA::CreateX();

	getchar();

	return 1;
}
