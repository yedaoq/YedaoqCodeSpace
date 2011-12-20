#pragma once

class CTwainScanistor
{
public:
	CTwainScanistor(CTwainSourceManager* pMgr, TW_IDENTITY id)
		: ManagerPtr(pMgr), ID(id)
	{}

	~CTwainScanistor(void);


protected:
	CTwainSourceManager*	ManagerPtr;
	TW_IDENTITY				ID;
};

