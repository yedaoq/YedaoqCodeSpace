/* ___________________________________________________________________________
@ 
@ file - CAtlComModuleScope.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-2-7
@ info -
@        在Scope对象生存期内提供ATL中的COM环境
@ -----------
@ 功能 : 初始化全局对象ATL::_pModule和ATL::_pAtlModule，并调用AtlAxWinInit注册AtlAxWinx窗口类
@ 要点 : 
@    1. 使用静态引用计数来在任意Scope生存期间维护COM环境，并且不重复初始化
@    2. CComModule和CAtlModule在其构造函数中设置ATL::_pModule和ATL::_pAtlModule全局变量，这意味着：
@         a. 若用户通过其它方式创建了Module对象的话，则此类不应该再创建Module对象，以免覆盖；
@         b. 若此类创建Module对象之后，用户再以其它方式创建Module对象的话，ATL::_pModule将不再指
@            向本类创建的Module实例。此类必须能够识别这种情况，它应当只销毁自己创建的那个实例，
@            也就是说不能直接销毁ATL::_pModule指向的对象。比如，若此类创建的对象在堆上，而用户在
@            栈上创建了另一个Module对象并将ATL::_pModule指向了它，则删除它将引发异常。
@       因此，此类除引用计数外，另外维护一个静态Module实例指针，在销毁将对比此指针与ATL::_pModule
@       的值，仅当它们一致时才清除ATL::_pModule指向的对象。
/* ___________________________________________________________________________*/

#pragma once
#include <atlbase.h>
#include <atlwin.h>

class CAtlComModuleScope
{
public:
	CAtlComModuleScope();
	~CAtlComModuleScope();

protected:
	static unsigned int		RefCount;
	static ATL::CComModule* InstancePtr;
};

__declspec(selectany) unsigned int CAtlComModuleScope::RefCount = 0;
__declspec(selectany) ATL::CComModule* CAtlComModuleScope::InstancePtr;

CAtlComModuleScope::CAtlComModuleScope()
{
	if(0 == RefCount)
	{
		if(!ATL::_pModule)
		{
			InstancePtr = ATL::_pModule = new ATL::CComModule();
		}
		AtlAxWinInit();
	}
	++RefCount; //InterlockedIncrement(RefCount);
}

CAtlComModuleScope::~CAtlComModuleScope()
{								
	if(1 == RefCount)					// 当计数值为1时，根据最高位决定是否销毁ATL::pModule
	{
		if(InstancePtr == ATL::_pModule && InstancePtr)
		{
			ATL::CComModule* pTemp = InstancePtr;
			ATL::_pAtlModule = 0;
			InstancePtr = ATL::_pModule = 0;			
			delete pTemp;
		}

		AtlAxWinTerm();
	}
	else if(0 == RefCount)				// 下溢出
	{
		return;
	}								
	--RefCount;							// 递减计数值
}
