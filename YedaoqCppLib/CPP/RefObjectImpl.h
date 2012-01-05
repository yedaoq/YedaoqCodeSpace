#pragma once
#include "IRefObject.h"

template <typename T>
class CRefObjectImpl : public T
{
//	OBJECT_REFINTERFACE_IMPLEMENTS
//OBJECT_REFINTERFACE_IMPLEMENTS宏等同于//////////////////////////之间的代码
//为了调试，此处将代码写在模板中
//////////////////////////////////////
public:
	ULONG AddRef()
	{
		return ::InterlockedIncrement( &m_cRef);
	}
	ULONG Release()
	{
		ULONG cRef = ::InterlockedDecrement( &m_cRef);
		if (cRef)
			return cRef;
		delete this;
		return  0;
	}
protected:
	volatile LONG m_cRef;
////////////////////////////////////////////
	CRefObjectImpl(void){m_cRef = 0;}
	virtual ~CRefObjectImpl(void){};


};
