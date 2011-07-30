#include "stdafx.h"
#include <iostream>

using namespace std;

class obj
{
public:
	obj()
	{
		cout<<"obj construct  ..."<<endl;
	}
	
	~obj()
	{
		cout<<"obj destruct ..."<<endl;
	}
};

void LocalVarIniTest1()
{
	cout<<"Local Variable Initial Test Function 1 : do{} "<<endl;
	
	int i = 2;

	do 
	{
		cout<<"Begin of the loop body ... "<<endl;
		obj o;
		cout<<"End of the loop body ..."<<endl;
	} while (--i);
	
	cout<<"End of Function body 1"<<endl<<endl;
}

void LocalVarIniTest2()
{
	cout<<"Local Variable Initial Test Function 2 : {} "<<endl;
	
	
	{
		cout<<"Begin of the loop body ... "<<endl;
		obj o;
		cout<<"End of the loop body ..."<<endl;
	} 
	
	cout<<"End of Function body 2"<<endl<<endl;
}

void LocalVarIniTest3()
{
	cout<<"Local Variable Initial Test Function 3 : if{} "<<endl;
	
	
	if(true) {
		cout<<"Begin of the loop body ... "<<endl;
		obj o;
		cout<<"End of the loop body ..."<<endl;
	} 
	
	cout<<"End of Function body 3"<<endl<<endl;
}

void LocalVarIniTest4()
{
	cout<<"Local Variable Initial Test Function 4 : no sub range "<<endl;
	
	obj o;
	
	cout<<"End of Function body 4"<<endl<<endl;
}

void LocalVarIniTest5()
{
	obj o;

	cout<<"Local Variable Initial Test Function 5 : no sub range "<<endl;
	
	cout<<"End of Function body 5"<<endl<<endl;
}

int mainLocalVarIni()
{
	LocalVarIniTest1();
	LocalVarIniTest2();
	LocalVarIniTest3();
	LocalVarIniTest4();
	LocalVarIniTest5();

	getchar();

	return 1;
}