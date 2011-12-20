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
	
public:
	~CTwainSourceManager(void);

	int						Load();
	int						UnLoad();

	int						OpenDSM();
	int						CloseDSM();

	int						PromptSelectSource(CTwainScanistor** ppScanistor);
	int						OpenSource(LPCTSTR lpScanistorId);
	IEnumerator<LPCTSTR>*	EnumSource();

	TW_UINT16 CallDSMEntry(pTW_IDENTITY pApp, pTW_IDENTITY pSrc, TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData);

	int MessageLevel (VOID)
	{
		return (MessageLevelVAR);
	}

	void SetMessageLevel (int Level)
	{
		MessageLevelVAR = Level;
		return;
	}

protected:
	bool			TWDSMOpen;		// glue code flag for an Open Source Manager
	HMODULE			DSMModule;		// handle to Source Manager for explicit load
	DSMENTRYPROC	DSMEntry;		// entry point to the SM
	TW_IDENTITY		AppID;			// twain application identify
	HWND			WndNotify;		// 

	int				MessageLevelVAR;
};

