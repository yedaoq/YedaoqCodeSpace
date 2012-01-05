#pragma once

interface IRefObject
{
	virtual ULONG AddRef() = 0;
	virtual ULONG Release() = 0;
};

#define OBJECT_REFINTERFACE_IMPLEMENTS public:\
ULONG AddRef(){return ::InterlockedIncrement( &m_cRef);}\
ULONG Release(){\
	ULONG cRef = ::InterlockedDecrement( &m_cRef);\
	if (cRef)\
		return cRef;\
	delete this;\
	return  0;}\
private:\
	volatile LONG m_cRef;


template <class T>
class CAutoRefPtr
{
	T* p;
public:
	CAutoRefPtr(){ p = NULL;}
	CAutoRefPtr(T* lp) { p = lp;if (p) p->AddRef();}
	CAutoRefPtr(const CAutoRefPtr<T>& lp) 
	{
		p = lp.p;
		if (p) p->AddRef();
	}
	~CAutoRefPtr()
	{
		if (p)
			p->Release();
	}

	operator const T*() const
	{
		return p;
	}

	operator T*()
	{
		return p;
	}

	T* operator=(T* lp)
	{
		if (p == lp)
			return p;
		
		if (p) p->Release();
		p = lp;
		if (p) p->AddRef();

		return p;
	}
	
	T* operator=(const CAutoRefPtr<T>& lp) throw()
	{
		if (p == lp.p)
			return p;

		if (p) p->Release();
		p = lp.p;
		if (p) p->AddRef();

		return p;
	}

	T* operator->()
	{
		return p;
	}

	bool operator!()
	{
		return (p == NULL);
	}
	bool operator<(const CAutoRefPtr<T>& o) const
	{
		return p < o.p;
	}
	bool operator<(const T* pT) const
	{
		return p < pT;
	}
	bool operator!=(const T* pT) const
	{
		return p != pT;
	}

	bool operator==(const T* pT) const
	{
		return p == pT;
	}

	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}

	void Attach(T* p2)
	{
		if (p)
			p->Release();
		p = p2;
	}
	// Detach the interface (does not Release)
	T* Detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}

};