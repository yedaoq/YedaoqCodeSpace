#include "StdAfx.h"
#include "TwainScanistor.h"
#include "TwainSourceManager.h"
#include <exception>

#define TWSX_UNKNOW 

CTwainScanistor::CTwainScanistor(CTwainSourceManager* pMgr, const TW_IDENTITY& id, bool open)
	: ManagerPtr_(pMgr), ID_(id), State_(EnumState::Closed), XferMode_()
{
	if(!pMgr)
	{
		throw std::exception();
	}

	if (open)
	{
		Open();
	}
}

TW_UINT16 CTwainScanistor::Call( TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData )
{
	return ManagerPtr_->CallDSMEntry(&ID_, DG, DAT, MSG, pData);
}

CTwainScanistor::~CTwainScanistor(void)
{

}

TW_INT16 CTwainScanistor::Open()
{
	TW_UINT16 twRC = TWRC_SUCCESS;

	if(EnumState::Closed != State_)
	{
		return twRC;
	}

	TW_UINT16 twRC = Call(DG_CONTROL, DAT_IDENTITY,	MSG_OPENDS, &ID_);
	if(TWRC_SUCCESS != twRC)
	{
		throw twRC;
		return twRC;
	}
	
	State_ = EnumState::Opened;	
}

TW_INT16 CTwainScanistor::Close()
{
	TW_UINT16 twRC = TWRC_SUCCESS;

	Disable();

	if(EnumState::Closed != State_)
	{
		twRC = Call(DG_CONTROL, DAT_IDENTITY,	MSG_CLOSEDS, &ID_);
		if(TWRC_SUCCESS == twRC)
		{
			State_ = EnumState::Closed;
		}
		else
		{
			throw twRC;
		}
	}
	return twRC;
}

TW_UINT16 CTwainScanistor::Enable()
{
	if(State_ != EnumState::Opened)
	{
		return TWRC_FAILURE;
	}

	TW_UINT16 twRC = Call(DG_CONTROL, DAT_USERINTERFACE, MSG_ENABLEDS, &Interface_);
	if(TWRC_SUCCESS == twRC)
	{
		State_ = EnumState::Enabled;
	}
	else
	{
		throw twRC;
	}

	return twRC;
}

TW_UINT16 CTwainScanistor::Disable()
{
	if(State_ != EnumState::Enabled || State_ != EnumState::Transferring)
	{
		return TWRC_FAILURE;
	}

	TW_UINT16 twRC = Call(DG_CONTROL, DAT_USERINTERFACE, MSG_DISABLEDS, &Interface_);
	if(TWRC_SUCCESS == twRC)
	{
		State_ = EnumState::Opened;
	}
	else
	{
		throw twRC;
	}

	return twRC;
}


void CTwainScanistor::OnEvent( TW_UINT16 msg )
{
	switch (msg)
	{
		case MSG_XFERREADY:
			if(EnumState::Enabled == State_)
			{
				TWTransferImage(hWnd);
			}			
			else
			{
				throw msg;
			}
			break;

		case MSG_CLOSEDSREQ:
		case MSG_CLOSEDSOK:
			Close();
			break;
			
	// No message from the Source to the App break;
	// possible new message
		case MSG_NULL:
		default:
			break;
	}   
}

TW_UINT16 CTwainScanistor::GetImage()
{
	TW_CAPABILITY twCapability;
	pTW_ENUMERATION pEnumeration = NULL;		
	TW_UINT16 index = 0, i = 0;
	TW_STR64 buffer;
	TW_UINT16 ModeType = 0;

	memset(buffer, 0, sizeof(TW_STR64));		
	memset(&twCapability, 0, sizeof(TW_CAPABILITY));
	twCapability.Cap = ICAP_XFERMECH;

	/*
	* Call the triplet with MSG_GET to get the current Value of 
	* ICAP_XFERMECH
	*/
	CallDSMEntry(&appID,
				&dsID,
				DG_CONTROL,
				DAT_CAPABILITY,
				MSG_GET,
				(TW_MEMREF)&twCapability);

	pEnumeration = (pTW_ENUMERATION)GlobalLock(twCapability.hContainer);

	for (index = 0;index < pEnumeration->NumItems; index++)
	{
		if(index == pEnumeration->CurrentIndex)
		{
			for(i = 0; i < MAX_SETUPXFERTYPE; i++)
			{
				if(*(TW_UINT16*)&pEnumeration->ItemList[index*(sizeof(TW_UINT16))] == SetupXferType[i].ItemId)
				{
					TRACE("Value = %s.\n", SetupXferType[i].pszItemName);
					ModeType = SetupXferType[i].ItemId;
					break;
				}
			}
		}
	}
	GlobalUnlock(twCapability.hContainer);
	GlobalFree(twCapability.hContainer);

	if(ModeType == TWSX_MEMORY)
	{
		DoMemTransfer(hWnd);
	}
	else if(ModeType == TWSX_FILE)
	{
		DoFileTransfer(hWnd);
	}
	else if(ModeType == TWSX_NATIVE)
	{
		DoNativeTransfer(hWnd);
	}
}

bool CTwainScanistor::SetXferMode( EnumXferMode mode )
{
	TW_CAPABILITY   cap;
	pTW_ONEVALUE    pval = NULL;
	TW_INT16        status = TWRC_FAILURE;  
	OPENFILENAME    ofn;
	char szDirName[256];
	char szFile[256];
	char szFileTitle[256];
	char chReplace = '|';
	char szFilter[256];
	UINT i = 0, cbString = 0;

	/*
	*	Initialize all structures
	*/
	memset(szDirName, 0, sizeof(char[256]));
	memset(szFile, 0, sizeof(char[256]));
	memset(szFileTitle, 0, sizeof(char[256]));
	memset(szFilter, 0, sizeof(char[256]));
	memset(&cap, 0, sizeof(TW_CAPABILITY));
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ASSERT(hWnd);

	LogMessage("TWXferMech\r\n");

	cap.Cap = ICAP_XFERMECH;
	cap.ConType = TWON_ONEVALUE;

	/*
	* alloc the container
	*/
	if (cap.hContainer = GlobalAlloc(GHND, sizeof(TW_ONEVALUE)))
	{
		pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
		pval->ItemType = TWTY_UINT16;

		if (GetMenuState (GetMenu (hWnd), TW_APP_BUFFER, MF_BYCOMMAND) & MF_CHECKED)       
		{
			/*
			* memory transfer
			*/
			pval->Item = TWSX_MEMORY;
		}
		else if (GetMenuState (GetMenu (hWnd), TW_APP_FILE, MF_BYCOMMAND) & MF_CHECKED)       
		{
			/*
			* file transfer
			*/
			pval->Item = TWSX_FILE;

			/*
			* file transfer currently fixed to bitmap format
			*
			* get the filename to save as
			* check formats supported by the source            
			*/

			/*
			*	Make sure only .bmp file extensions are possible
			*/
			GetCurrentDirectory(sizeof(szDirName), szDirName);
			szFile[0] = '\0';
			cbString = LoadString(hInst, IDS_FILTERSTRING, szFilter, 
										sizeof(szFilter));
			for(i = 0;szFilter[i] != '\0';i++)
			{
				if(szFilter[i] == chReplace)
				{
					szFilter[i] = '\0';
				}
			}

			memset(&ofn, 0, sizeof(OPENFILENAME));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.hInstance = NULL;
			ofn.lpstrFilter = szFilter;
			ofn.lpstrCustomFilter = (LPSTR)NULL;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFileTitle = szFileTitle;
			ofn.nMaxFileTitle = sizeof(szFileTitle);
			ofn.lpstrInitialDir = szDirName;
			ofn.lpstrTitle = (LPSTR)NULL;
			ofn.Flags = OFN_SHOWHELP|OFN_OVERWRITEPROMPT;
			ofn.nFileOffset = 0;
			ofn.nFileExtension = 0;
			ofn.lpstrDefExt = (LPSTR)"bmp";
			ofn.lCustData = 0L;
			ofn.lpfnHook = NULL;
			ofn.lpTemplateName = (LPSTR)NULL;

			if (GetSaveFileName(&ofn)==FALSE)
			{
				GlobalUnlock(cap.hContainer);
				GlobalFree(cap.hContainer);
				cap.hContainer = NULL;
				return(TWRC_FAILURE);
			}
			else
			{
				lstrcpy(Filename, szFileTitle);
			}
		}
		else 
		{
			pval->Item = TWSX_NATIVE;
		}

		GlobalUnlock(cap.hContainer);

		status = CallDSMEntry(&appID,
						&dsID,
						DG_CONTROL, 
						DAT_CAPABILITY, 
						MSG_SET,
						(TW_MEMREF)&cap);

		GlobalFree((HANDLE)cap.hContainer);

		if (status != TWRC_SUCCESS)
		{
			if (MessageLevel() >= ML_ERROR)
			{
				ShowRC_CC(hWnd, 1, status, 1, "","MSG_SET of ICAP_XFERMECH"); 
			}
		} 
	}
	else
	{
		status = TWRC_FAILURE;
		if (MessageLevel() >= ML_ERROR)
		{
			ShowRC_CC(hWnd, 0, 0, 0, "Memory Allocation Failed","MSG_SET of ICAP_XFERMECH"); 
		}
	}
	return status;
}

TW_UINT16 CTwainScanistor::SetCapbility( TW_UINT16 id, const CCapDataContainerBase& data )
{
	TW_CAPABILITY cap = { id, data.ConTypeID(),	data.MakeGlobalMemory() };
	_ASSERT(cap.hContainer);
	Call(DG_CONTROL, DAT_CAPABILITY, MSG_SET, &cap);
}

TW_UINT16 CTwainScanistor::GetCapbility( TW_UINT16 id, TW_CAPABILITY& cap )
{
	if(cap.hContainer)
	{
		::GlobalFree(cap.hContainer);
	}
	cap.Cap = id;
	cap.ConType = TWON_DONTCARE16;
	cap.hContainer = NULL;
	return Call(DG_CONTROL, DAT_CAPABILITY, MSG_GET, &cap);
}
