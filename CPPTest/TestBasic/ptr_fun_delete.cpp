#include "stdafx.h"
#include<stdio.h>
#include <iostream>
#include <memory>
#include <functional>

using namespace std;

class TypeA
{
public:
	~TypeA()
	{
		cout<<"destruct"<<endl;
	}
};

template<typename T>
int Delete(T* obj)
{
	delete obj;
	return 1;
}

template int Delete(TypeA*);

int mainPtrFunDelete()
{
	TypeA* obj = new TypeA();
	//std::ptr_fun(::delete<TypeA>)(obj);
	std::ptr_fun(Delete<TypeA>)(obj);

	return 1;
}
