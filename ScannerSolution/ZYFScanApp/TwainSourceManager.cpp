#include "StdAfx.h"
#include "TwainSourceManager.h"
#include "TwainScanistor.h"

#define DSMDLLFILE		TEXT("TWAIN_32.DLL")
#define DSMENTRYNAME	("DSM_Entry")
#define VALID_HANDLE    32      // valid windows handle SB >= 32

CTwainSourceManager::CTwainSourceManager(void)
{
}


CTwainSourceManager::~CTwainSourceManager(void)
{
	if(TWDSMOpen)
	{
		UnLoad();
	}
}

int CTwainSourceManager::PromptSelectSource( CTwainScanistor** ppScanistor )
{
	if(!TWDSMOpen || Load() != 1)
	{
		return -1;
	}

	TW_UINT16 twRC = TWRC_FAILURE;
	TW_IDENTITY NewDSIdentity;

	memset(&NewDSIdentity, 0, sizeof(TW_IDENTITY));

	//if (TWDSOpen)
	//{
	//	LogMessage("TWSelectDS -- source already open\r\n");

	//	//A Source is already open
	//	if (MessageLevel() >= ML_ERROR)
	//	{
	//		ShowRC_CC(hWnd, 0, 0, 0,
	//			"A Source is already open\nClose Source before Selecting a New Source",
	//			"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
	//	}
	//	twRC = TWRC_FAILURE;
	//}
	//else
	//{
		// I will settle for the system default.  Shouldn't I get a highlight
		// on system default without this call?

		twRC = CallDSMEntry(&appID,
			NULL,
			DG_CONTROL,
			DAT_IDENTITY,
			MSG_GETDEFAULT,
			(TW_MEMREF)&NewDSIdentity);

		// This call performs one important function:
		// - should cause SM to put up dialog box of available Source's
		// - tells the SM which application, appID.id, is requesting, REQUIRED
		// - returns the SM assigned NewDSIdentity.id field, you check if changed
		//  (needed to talk to a particular Data Source)
		// - be sure to test return code, failure indicates SM did not close !!
		//
		twRC = CallDSMEntry(&appID,
			NULL,
			DG_CONTROL,
			DAT_IDENTITY,
			MSG_USERSELECT,
			(TW_MEMREF)&NewDSIdentity);

		// Check if the user changed the Source and react as apporpriate.
		// - TWRC_SUCCESS, log in new Source
		// - TWRC_CANCEL,  keep the current Source
		// - default,      check down the codes in a status message, display result
		//

		switch (twRC)
		{
		case TWRC_SUCCESS:
			dsID = NewDSIdentity; 
			if (MessageLevel() >= ML_INFO)
			{
				ShowTW_ID(hWnd, dsID,
					"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
			}
			break;
		case TWRC_CANCEL:
			if (MessageLevel() >= ML_INFO)
			{
				ShowRC_CC(hWnd, 1, twRC, 0,
					"",
					"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
			}
			break;
		case TWRC_FAILURE:	        
		default:
			if (MessageLevel() >= ML_ERROR)
			{
				ShowRC_CC(hWnd, 1, twRC, 0,
					"",
					"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
			}
			break;
		}
	}
	return (twRC);
}

int CTwainSourceManager::OpenSource( LPCTSTR lpScanistorId )
{
	if(!TWDSMOpen || Load() != 1)
	{
		return -1;
	}
}

IEnumerator<LPCTSTR>* CTwainSourceManager::EnumSource()
{
	if(!TWDSMOpen || Load() != 1)
	{
		return 0;
	}
}

int CTwainSourceManager::Load()
{
	TCHAR szBuf[MAX_PATH] = {0};
	GetWindowsDirectory(szBuf, MAX_PATH);
	PathAppend(szBuf, DSMDLLFILE);

	if(	(DSMModule = LoadLibrary(szBuf)) != NULL
		&& DSMModule >= (HANDLE)VALID_HANDLE
		&& (DSMEntry = (DSMENTRYPROC)GetProcAddress(DSMModule, DSMENTRYNAME)) != NULL)
	{
		this->TWDSMOpen = true;
		return 1;
	}
	else
	{
		UnLoad();
		return -1;
	}
}

int CTwainSourceManager::UnLoad()
{
	if(DSMModule > (HANDLE)VALID_HANDLE)
	{
		FreeLibrary(DSMModule);
	}

	DSMModule = NULL;
	DSMEntry = NULL;
	TWDSMOpen = false;

	return 1;
}

/*
* Fucntion: CallDSMEntry
* Author:	Nancy Letourneau / J.F.L. Peripherals Inc.
* Input:  
*		Function - 
*		pApp - 
*		pSrc - 
*		DG -
*		DAT -
*		MSG -
*		pData -
* Output: 
*		TW_UINT16 - Value of Item field of container. 
* Comments:
*
*/
TW_UINT16 CTwainSourceManager::CallDSMEntry(pTW_IDENTITY pApp, pTW_IDENTITY pSrc,
	TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData)
{
	TW_UINT16 twRC = (*DSMEntry)(pApp, pSrc, DG, DAT, MSG, pData);

	if((twRC != TWRC_SUCCESS)&&(DAT!=DAT_EVENT))
	{
		VERIFY((*DSMEntry)(pApp, pSrc, DG_CONTROL, DAT_STATUS, MSG_GET, (TW_MEMREF)&gGlobalStatus) == TWRC_SUCCESS);
		
		TRACE("CallDSMEntry function: call failed with RC = %d, CC = %d.\n", twRC, gGlobalStatus.ConditionCode);
	}
	return twRC;
}