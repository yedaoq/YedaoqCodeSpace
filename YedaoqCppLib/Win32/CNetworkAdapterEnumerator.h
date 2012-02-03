/* ___________________________________________________________________________
@ 
@ file - CNetworkAdapterEnumerator.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-29
@ info -
@     Íø¿¨Ã¶¾ÙÆ÷
/* ___________________________________________________________________________*/

#include <Iphlpapi.h>
#include "Enumerator.h"

#pragma comment(lib,"Iphlpapi.lib")

class CNetworkAdapterEnumerator : public CEnumeratorBase<IP_ADAPTER_INFO>
{
public:
	CNetworkAdapterEnumerator()
		: pBuf(0), pCur(0)
	{}

	~CNetworkAdapterEnumerator()
	{
		if(pBuf)
		{
			free(pBuf);
			pBuf = 0;
		}
	}

	virtual bool MoveNext()
	{
		if(!pBuf)
		{
			return Initialize();
		}
		else if(pCur)
		{
			return 0 == (pCur = pCur->Next);
		}
		return false;
	}

	virtual const IP_ADAPTER_INFO& Current()
	{
		return *pCur;
	}

	virtual void Reset()
	{
		pCur = pBuf;
	}

	virtual ICloneable* Clone() const { return new CNetworkAdapterEnumerator(); }

protected:
	bool Initialize()
	{
		ULONG ulSize = 0;
		do 
		{
			GetAdaptersInfo(NULL, &ulSize);
			if(ulSize <= 0) break;

			pBuf = (PIP_ADAPTER_INFO)malloc(ulSize);
			if(!pBuf) break;

			if(GetAdaptersInfo(pBuf, &ulSize) != ERROR_SUCCESS) break;
			pCur = pBuf;

		} while (false);
		return 0 == pCur;
	}

protected:
	PIP_ADAPTER_INFO pBuf;
	PIP_ADAPTER_INFO pCur;
};