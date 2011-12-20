#include "StdAfx.h"
#include "TwainSourceManager.h"
#include "TwainScanistor.h"

#define DSMDLLFILE		TEXT("TWAIN_32.DLL")
#define DSMENTRYNAME	("DSM_Entry")
#define VALID_HANDLE    32      // valid windows handle SB >= 32

CTwainSourceManager::CTwainSourceManager(void)
{
	AppID.Id = 0;
	AppID.Id = 0; 				// init to 0, but Source Manager will assign real value
	AppID.Version.MajorNum = 1;
	AppID.Version.MinorNum = 703;
	AppID.Version.Language = TWLG_USA;
	AppID.Version.Country  = TWCY_USA;
#ifdef WIN32
	lstrcpyA (AppID.Version.Info,  "TWAIN_32 Twacker 1.7.0.3  01/18/1999");
	lstrcpyA (AppID.ProductName,   "TWACKER_32");
#else
	lstrcpyA (AppID.Version.Info,  "TWAIN Twacker 1.7.0.3  01/18/1999");
	lstrcpyA (AppID.ProductName,   "TWACKER_16");
#endif

	AppID.ProtocolMajor = 1;//TWON_PROTOCOLMAJOR;
	AppID.ProtocolMinor = 7;//TWON_PROTOCOLMINOR;
	AppID.SupportedGroups =  DG_IMAGE | DG_CONTROL;
	lstrcpyA (AppID.Manufacturer,  "TWAIN Working Group");
	lstrcpyA (AppID.ProductFamily, "TWAIN Toolkit");

	MessageLevelVAR = ERROR;
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
	if(!TWDSMOpen || Load() != 1 || !ppScanistor)
	{
		return -1;
	}

	TW_UINT16 twRC = TWRC_FAILURE;
	TW_IDENTITY NewDSIdentity;

	memset(&NewDSIdentity, 0, sizeof(TW_IDENTITY));

	twRC = CallDSMEntry(&AppID,
		NULL,
		DG_CONTROL,
		DAT_IDENTITY,
		MSG_GETDEFAULT,
		(TW_MEMREF)&NewDSIdentity);

	twRC = CallDSMEntry(&AppID,
		NULL,
		DG_CONTROL,
		DAT_IDENTITY,
		MSG_USERSELECT,
		(TW_MEMREF)&NewDSIdentity);

	switch (twRC)
	{
	case TWRC_SUCCESS:
		*ppScanistor = new CTwainScanistor(this, NewDSIdentity);
		/*if (MessageLevel() >= ML_INFO)
		{
			ShowTW_ID(hWnd, dsID,
				"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
		}*/
		break;
	case TWRC_CANCEL:
		/*if (MessageLevel() >= ML_INFO)
		{
			ShowRC_CC(hWnd, 1, twRC, 0,
				"",
				"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
		}*/
		break;
	case TWRC_FAILURE:	        
	default:
		/*if (MessageLevel() >= ML_ERROR)
		{
			ShowRC_CC(hWnd, 1, twRC, 0,
				"",
				"DG_CONTROL/DAT_IDENTITY/MSG_USERSELECT");
		}*/
		break;
	}
	
	return (twRC == TWRC_SUCCESS) ? 1 : -1;
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

	return 0;
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

int CTwainSourceManager::OpenDSM()
{
	if(!DSMEntry && Load() != 1)
	{
		return -1;
	}

	TW_UINT16 twRC = CallDSMEntry(&AppID,
		NULL,
		DG_CONTROL,
		DAT_PARENT,
		MSG_OPENDSM,
		(TW_MEMREF)&WndNotify);

	switch (twRC)
	{
	case TWRC_SUCCESS:
		// Needed for house keeping.  Do single open and do not
		// close SM which is not already open ....
		TWDSMOpen = true;
		if (MessageLevel() >= ML_FULL)
		{
			ShowRC_CC(hWnd, 0, 0, 0, 
				"Source Manager was Opened successfully", 
				"TWAIN Information");
		}
		break;

	case TWRC_FAILURE:
		LogMessage("OpenDSM failure\r\n");
	default:
		// Trouble opening the SM, inform the user
		TWDSMOpen = FALSE;
		if (MessageLevel() >= ML_ERROR)                
		{
			ShowRC_CC(hWnd, 1, twRC, 0,	//Source Manager
				"",
				"DG_CONTROL/DAT_PARENT/MSG_OPENDSM");
		}
		break;
	}


}

int CTwainSourceManager::CloseDSM()
{
	TW_UINT16 twRC = TWRC_FAILURE;
	char buffer[80];

	memset(buffer, 0, sizeof(char[80]));

	if (!TWDSMOpen)
	{
		if (MessageLevel()  >= ML_ERROR)
		{
			ShowRC_CC(hWnd, 0, 0, 0,
				"Cannot Close Source Manager\nSource Manager Not Open", 
				"Sequence Error");
		}
	}
	else
	{
		if (TWDSOpen==TRUE)
		{
			if (MessageLevel()  >= ML_ERROR)
			{
				ShowRC_CC(hWnd, 0, 0, 0,
					"A Source is Currently Open", "Cannot Close Source Manager");
			}
		}
		else
		{
			// Only close something which is already open
			if (TWDSMOpen==TRUE)
			{
				// This call performs one important function:
				// - tells the SM which application, appID.id, is requesting SM to close
				// - be sure to test return code, failure indicates SM did not close !!

				twRC = CallDSMEntry(&appID,
					NULL,
					DG_CONTROL,
					DAT_PARENT,
					MSG_CLOSEDSM,
					&hWnd);

				if (twRC != TWRC_SUCCESS)
				{
					// Trouble closing the SM, inform the user
					if (MessageLevel() >= ML_ERROR)
					{
						ShowRC_CC(hWnd, 1, twRC, 0,
							"",
							"DG_CONTROL/DAT_PARENT/MSG_CLOSEDSM");
					}

					wsprintf(buffer,"CloseDSM failure -- twRC = %d\r\n",twRC);
					LogMessage(buffer);
				}
				else
				{
					TWDSMOpen = FALSE;
					// Explicitly free the SM library
					if (hDSMDLL)
					{        
						FreeLibrary (hDSMDLL);
						hDSMDLL=NULL;
						// the data source id will no longer be valid after
						// twain is killed.  If the id is left around the
						// data source can not be found or opened
						dsID.Id = 0;  
					}
					if (MessageLevel() >= ML_FULL)
					{
						ShowRC_CC(hWnd, 0, 0, 0,
							"Source Manager was Closed successfully", 
							"TWAIN Information");
					}
				}
			}
		}
	}
	// Let the caller know what happened
	return (twRC==TWRC_SUCCESS);
}