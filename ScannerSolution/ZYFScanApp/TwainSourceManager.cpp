#include "StdAfx.h"
#include "TwainSourceManager.h"
#include "TwainScanistor.h"
#include <memory>
#include <atlconv.h>
#include <string>
#include "..\twcommon\Captest.h"
#include "..\twcommon\twndebug.h"
#include <winerror.h>

#define DSMDLLFILE		TEXT("TWAIN_32.DLL")
#define DSMENTRYNAME	("DSM_Entry")
#define VALID_HANDLE    (HANDLE)32      // valid windows handle SB >= 32
#define NOTIFYWNDCLASSNAME TEXT("TWAIN_WndNotify")

CTwainSourceManager::CTwainSourceManager(void)
	: State(EnumState::Prepare)
	, DSMModule(0)
	, DSMEntry(0)
	, WndNotify(0)
	, MessageLevelVAR(ERROR)
{
	// 初始化AppID
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

	// 注册响应窗口类
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));

	wc.hInstance = ::GetModuleHandleA(NULL);
	wc.lpfnWndProc = WndNotifyProc;
	wc.lpszClassName = NOTIFYWNDCLASSNAME;

	if(RegisterClass(&wc))
	{
		throw E_FAIL;
	}
}


CTwainSourceManager::~CTwainSourceManager(void)
{
	if(DSMOpen == State) CloseDSM();
	if(DSMLoad == State) UnLoad();
}

int CTwainSourceManager::PromptSelectSource( CTwainScanistor** ppScanistor )
{
	if((State != DSMOpen || OpenDSM() != 1) || !ppScanistor)
	{
		return -1;
	}

	TW_UINT16 twRC = TWRC_FAILURE;
	TW_IDENTITY NewDSIdentity;

	memset(&NewDSIdentity, 0, sizeof(TW_IDENTITY));

	twRC = CallDSMEntry(NULL,
		DG_CONTROL,
		DAT_IDENTITY,
		MSG_GETDEFAULT,
		(TW_MEMREF)&NewDSIdentity);

	twRC = CallDSMEntry(NULL,
		DG_CONTROL,
		DAT_IDENTITY,
		MSG_USERSELECT,
		(TW_MEMREF)&NewDSIdentity);

	switch (twRC)
	{
	case TWRC_SUCCESS:
		*ppScanistor = new CTwainScanistor(this, NewDSIdentity);
		break;
	case TWRC_CANCEL:
		break;
	case TWRC_FAILURE:	        
	default:
		break;
	}
	
	return (twRC == TWRC_SUCCESS) ? 1 : -1;
}

CTwainScanistor* CTwainSourceManager::OpenSource( LPCTSTR lpProductName )
{
	if(State != DSMOpen || OpenDSM() != 1)
	{
		return 0;
	}

	if(!lpProductName || !(*lpProductName))
	{
		return 0;
	}

	std::auto_ptr<IEnumerator<TW_IDENTITY>> pEnumSrc(EnumSource());
	if(pEnumSrc.get())
	{
		std::string productName = T2A(lpProductName);
		while(pEnumSrc->MoveNext())
		{
			if(productName == pEnumSrc->Current().ProductName)
			{
				return OpenSource(pEnumSrc->Current());
			}
		}
	}
	return 0;
}

CTwainScanistor* CTwainSourceManager::OpenSource( const TW_IDENTITY& sourceID )
{
	if(State != DSMOpen || OpenDSM() != 1)
	{
		return 0;
	}

	for (ScanistorList::iterator iter = OpenScanistors.begin(); iter != OpenScanistors.end(); ++iter)
	{
		if(sourceID.Id == (*iter)->ID().Id)
		{
			return *iter;
		}
	}

	return new CTwainScanistor(this, sourceID, true);
}

IEnumerator<TW_IDENTITY>* CTwainSourceManager::EnumSource()
{
	if(State != DSMOpen || OpenDSM() != 1)
	{
		return 0;
	}

	return new CTwainSourceEnumerator(this);
}

int CTwainSourceManager::Load()
{
	if(State != Prepare) return 1;

	TCHAR szBuf[MAX_PATH] = {0};
	GetWindowsDirectory(szBuf, MAX_PATH);
	PathAppend(szBuf, DSMDLLFILE);

	if(	(DSMModule = LoadLibrary(szBuf)) != NULL
		&& DSMModule >= (HANDLE)VALID_HANDLE
		&& (DSMEntry = (DSMENTRYPROC)GetProcAddress(DSMModule, DSMENTRYNAME)) != NULL)
	{
		State = DSMLoad;
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
	if(State == DSMOpen) CloseDSM();

	if(DSMModule > (HANDLE)VALID_HANDLE)
	{
		FreeLibrary(DSMModule);
	}

	DSMModule = NULL;
	DSMEntry = NULL;
	State = Prepare;

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
TW_UINT16 CTwainSourceManager::CallDSMEntry(pTW_IDENTITY pSrc,
	TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData)
{
	TW_UINT16 twRC = (*DSMEntry)(&AppID, pSrc, DG, DAT, MSG, pData);

	if((twRC != TWRC_SUCCESS)&&(DAT!=DAT_EVENT))
	{
		VERIFY((*DSMEntry)(&AppID, pSrc, DG_CONTROL, DAT_STATUS, MSG_GET, (TW_MEMREF)&gGlobalStatus) == TWRC_SUCCESS);
		
		TRACE("CallDSMEntry function: call failed with RC = %d, CC = %d.\n", twRC, gGlobalStatus.ConditionCode);
	}
	return twRC;
}

int CTwainSourceManager::OpenDSM()
{
	if(DSMOpen == State)
	{
		return 1;
	}

	if(State != DSMLoad || Load() != 1)
	{
		return -1;
	}

	TW_UINT16 twRC = CallDSMEntry(NULL,
		DG_CONTROL,
		DAT_PARENT,
		MSG_OPENDSM,
		(TW_MEMREF)&WndNotify);

	switch (twRC)
	{
	case TWRC_SUCCESS:
		// Needed for house keeping.  Do single open and do not
		// close SM which is not already open ....
		State = DSMOpen;
		/*if (MessageLevel() >= ML_FULL)
		{
			ShowRC_CC(hWnd, 0, 0, 0, 
				"Source Manager was Opened successfully", 
				"TWAIN Information");
		}*/
		break;

	case TWRC_FAILURE:
		//LogMessage("OpenDSM failure\r\n");
	default:
		// Trouble opening the SM, inform the user
		//TWDSMOpen = FALSE;
		//if (MessageLevel() >= ML_ERROR)                
		//{
		//	ShowRC_CC(hWnd, 1, twRC, 0,	//Source Manager
		//		"",
		//		"DG_CONTROL/DAT_PARENT/MSG_OPENDSM");
		//}
		break;
	}

	return (State == DSMOpen) ? 1 : -1;
}

int CTwainSourceManager::CloseDSM()
{
	if(DSMOpen != State)
	{
		return 1;
	}

	TW_UINT16 twRC = TWRC_FAILURE;
	char buffer[80];

	memset(buffer, 0, sizeof(buffer));

	// This call performs one important function:
	// - tells the SM which application, appID.id, is requesting SM to close
	// - be sure to test return code, failure indicates SM did not close !!

	twRC = CallDSMEntry(NULL,
		DG_CONTROL,
		DAT_PARENT,
		MSG_CLOSEDSM,
		&WndNotify);

	if (twRC != TWRC_SUCCESS)
	{
 		//wsprintf(buffer,"CloseDSM failure -- twRC = %d\r\n",twRC);
 		//LogMessage(buffer);
	}
	else
	{
		State = DSMLoad;
	}

	// Let the caller know what happened
	return (twRC==TWRC_SUCCESS) ? 1 : -1;
}

int CTwainSourceManager::CreateNotifyWindow()
{
	if(WndNotify != 0)
	{
		throw E_FAIL;
	}

	WndNotify = CreateWindow(NOTIFYWNDCLASSNAME, NULL, NULL, 0, 0, 0, 0, NULL, NULL, ::GetModuleHandle(NULL), NULL);
	if(WndNotify < VALID_HANDLE)
	{
		throw E_FAIL;
	}

	return 1;
}

int CTwainSourceManager::DestroyNotifyWindow()
{
	if(0 == WndNotify)
	{
		throw E_FAIL;
	}

	DestroyWindow(WndNotify);
	WndNotify = 0;
}

LRESULT CALLBACK CTwainSourceManager::WndNotifyProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	MSG			msg = {hwnd, uMsg, wParam, lParam};
	TW_UINT16	twRC = TWRC_NOTDSEVENT;
	TW_EVENT	twEvent;
	CTwainSourceManager& twMgr = CTwainSourceManager::GetInstance();

	twEvent.TWMessage = MSG_NULL;
	twEvent.pEvent = (TW_MEMREF)&msg;

	for (ScanistorList::iterator iter = twMgr.OpenScanistors.begin(); iter != twMgr.OpenScanistors.end(); ++iter)
	{
		if((*iter)->State() != CTwainScanistor::EnumState::Closed)
		{
			twRC = twMgr.CallDSMEntry((pTW_IDENTITY)&((*iter)->ID()),
				DG_CONTROL, 
				DAT_EVENT,
				MSG_PROCESSEVENT, 
				(TW_MEMREF)&twEvent);

			if(TWRC_DSEVENT == twRC)
			{
				(*iter)->OnEvent(twEvent.TWMessage);
				return TRUE;
			}
		}		
	}
	
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

void CTwainSourceManager::OnScanistorCreate(CTwainScanistor *source)
{
	for (ScanistorList::iterator iter = OpenScanistors.begin(); iter != OpenScanistors.end(); ++iter)
	{
		if(source->ID().Id == (*iter)->ID().Id)
		{
			throw E_FAIL;
			return;
		}
	}

	OpenScanistors.push_back(source);
}

void CTwainSourceManager::OnScanistorDestroy(CTwainScanistor *source)
{
	for (ScanistorList::iterator iter = OpenScanistors.begin(); iter != OpenScanistors.end(); ++iter)
	{
		if(source->ID().Id == (*iter)->ID().Id)
		{
			iter = OpenScanistors.erase(iter);
			break;
		}
	}
}

bool CTwainSourceEnumerator::MoveNext()
{
	TW_UINT16 twRC = SourceMgr->CallDSMEntry(NULL, DG_CONTROL, DAT_IDENTITY, MsgID, &SourceID);
	switch (twRC)
	{
	case TWRC_SUCCESS:
		MsgID = MSG_GETNEXT;
		return true;

	case TWRC_FAILURE:
		// no next
		return false;
	default:
		// Trouble enumerate the source, inform the user
		break;
	}

	return false;
}

const TW_IDENTITY& CTwainSourceEnumerator::Current()
{
	if(MsgID == MSG_GETFIRST)
	{
		throw std::exception(/*TEXT("should execute MoveNext before get Current Object!")*/);
	}
	return SourceID;
}

void CTwainSourceEnumerator::Reset()
{
	MsgID = MSG_GETFIRST;
}