// TestDeleteThis.cpp : Defines the entry point for the console application.
//

//#define  _MT

#include "stdafx.h"
#include <windows.h>
#include <process.h>

class Test1
{
public:
	int m_X;

	void Destroy()
	{
		//delete this;		//OK
		DestroyInstance();
		Sleep(2000);
	};

protected:
	void DestroyInstance()
	{
		delete this;		//OK
	}
};

void DestroyProc(void * pParam)
{
	Test1 *v = (Test1 *)pParam;

	v->Destroy();

	return;
}

int main(int argc, char* argv[])
{
	Test1 *v = new Test1();

	//v->Destroy();		//OK : Destroy at the Creater Thread

	HANDLE tID = (HANDLE)_beginthread(DestroyProc, 0, v);

	WaitForSingleObject(tID,3000);

	return 0;
}

