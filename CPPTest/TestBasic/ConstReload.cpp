#include "stdafx.h"
#include<stdio.h>
#include <iostream>

using namespace std;

class TestConstReload
{
public :
	int& val() const
	{ 
		cout<<"const"<<endl;
		return const_cast<TestConstReload&>(*this).v;
	}

	int& val()
	{
		cout<<"non-const"<<endl;
		return v; 
	}

private:
	int v;
};

int mainConstReload()
{	
	/*non const*/
	TestConstReload a;

	cout<<a.val()<<endl;
	a.val() = 3;
	cout<<a.val()<<endl;

	// const
	const TestConstReload b;
	cout<<b.val()<<endl;
	b.val() = 4;
	cout<<b.val()<<endl;
	
	getchar();
	
	return 1;
}
