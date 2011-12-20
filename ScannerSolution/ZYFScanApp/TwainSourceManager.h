#pragma once

#include "Singlton.h"
#include "Enumerator.h"
#include "..\twcommon\Twain.h"
#include "TwainScanistor.h"

class CTwainSourceManager : public CSingleton<CTwainSourceManager>
{
	friend class CSingleton<CTwainSourceManager>;
protected:
	CTwainSourceManager(void);
	~CTwainSourceManager(void);

public:
	int						Load();
	int						UnLoad();

	int						PromptSelectSource(CTwainScanistor** ppScanistor);
	int						OpenSource(LPCTSTR lpScanistorId);
	IEnumerator<LPCTSTR>*	EnumSource();

	TW_UINT16 CallDSMEntry(pTW_IDENTITY pApp, pTW_IDENTITY pSrc, TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData);

protected:
	bool			TWDSMOpen;		// glue code flag for an Open Source Manager
	HMODULE			DSMModule;		// handle to Source Manager for explicit load
	DSMENTRYPROC	DSMEntry;		// entry point to the SM


};

