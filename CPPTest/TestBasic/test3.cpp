#include "stdafx.h"
#include <iostream>
#include "test.h"

using namespace std;

struct sa
{
	sa()
	{
		cout<<"construct sa"<<endl;
	}

	virtual ~sa()
	{
		cout<<"destruct sa"<<endl;
	}
};

struct sb : sa
{
	sb()
	{
		cout<<"construct sb"<<endl;
	}

	virtual ~sb()
	{
		cout<<"destruct sb"<<endl;
	}
};

void test3()
{
	sa *s = new sb();

	delete s;
}

