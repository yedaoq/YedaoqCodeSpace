#pragma once
#include "..\twcommon\Twain.h"

class CTwainSourceManager;

class CTwainScanistor
{
public:
	CTwainScanistor(CTwainSourceManager* pMgr, const TW_IDENTITY& id, bool open = true)
		: ManagerPtr(pMgr), ID(id)
	{
		if(!pMgr)
		{
			throw std::exception();
		}
	}

	~CTwainScanistor(void);

	TW_UINT16	Call(TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData);

	int			Open();
	int			Close();


protected:
	CTwainSourceManager*	ManagerPtr;
	TW_IDENTITY				ID;
	
};

