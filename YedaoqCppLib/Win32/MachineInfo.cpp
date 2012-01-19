
#include "StdAfx.h"
#include "MachineInfo.h"
#include "Assist.h"
#include "HexCoder.h"
#include "ErrorCode.h"

#pragma comment(lib,"Iphlpapi.lib")

BYTE DefaultAdapterAddress[MAX_ADAPTER_ADDRESS_LENGTH];

//////////////////////////////////////////////////////////////////////////
// EnumdapAters : enumerate all the adapters of the host
// args : 
// callback		- the callback that capture the adapter info
// return : return the count of adapters that has been enumerated(not the count of adapters that exist) when success
//////////////////////////////////////////////////////////////////////////
INT _stdcall EnumdAdapters(AdapterDeal callback)
{
	INT iReturn = MYERROR_NODEFINE;
	ULONG ulSize = 0;
	PIP_ADAPTER_INFO pInfo = NULL;

	if(NULL == callback) return MYERROR_INVALID_PARA;

	do 
	{
		GetAdaptersInfo(NULL, &ulSize);

		if(ulSize <= 0) break;
		
		pInfo = (PIP_ADAPTER_INFO)malloc(ulSize);
		
		if(pInfo == NULL) break;
		
		if(GetAdaptersInfo(pInfo, &ulSize) != ERROR_SUCCESS) break;
		
		iReturn = 0;
		while (pInfo)
		{
			++iReturn;
			if(!callback(pInfo)) break;
			pInfo = pInfo->Next;
		}
	} while (FALSE);
	
    if(NULL != pInfo)
	{
		free(pInfo);
		pInfo = NULL;
	}
	
	return iReturn;
}

//////////////////////////////////////////////////////////////////////////
// GetHostMacs : get the hex string list of all the adapter's mac address,separated in '|'
// args :
// lpBuffer		- out, the buffer receives the hex string
// iLen			- the length of the lpBuffer, in bytes
// return : return the hex string length of the mac addresses when success, and return a negative when error
//////////////////////////////////////////////////////////////////////////
INT _stdcall GetHostMacs(LPTSTR lpBuffer, INT iLen)
{
	return MYERROR_NODEFINE;
}

//////////////////////////////////////////////////////////////////////////
// GetDefaultMac : get the hex string of mac address of the first adapter
// Args :
// lpBuffer		- out, the buffer receives the hex string of the mac address
// iLen			- the length of the lpBuffer, in TCHARs
// Return : return the hex string length of the mac address when success, and return a negative when error
//////////////////////////////////////////////////////////////////////////
INT _stdcall GetDefaultMac(LPTSTR lpBuffer, INT iLen)
{
	if(lpBuffer == NULL) return MYERROR_INVALID_PARA;
	if(iLen < MAX_ADAPTER_ADDRESS_LENGTH * 2) return MYERROR_BUFFERLACK;

	ULONG ulSize=0;
	INT iReturn = MYERROR_NODEFINE;
	PIP_ADAPTER_INFO pInfo = NULL;

	do 
	{
		//if(GetAdaptersInfo(NULL, &ulSize) != ERROR_SUCCESS) return MYERROR_NODEFINE; // Get the AdapterInfo Size
		
		GetAdaptersInfo(NULL, &ulSize);

		if(ulSize <= 0) break;
		
		pInfo = (PIP_ADAPTER_INFO)malloc(ulSize);
		
		if(pInfo == NULL) break;
		
		if(GetAdaptersInfo(pInfo, &ulSize) != ERROR_SUCCESS) break;
		
		iReturn = Blob2HexStr(pInfo->Address, pInfo->AddressLength, lpBuffer, iLen);

	} while (FALSE);

	if(MYERROR_SUCCESS == iReturn) iReturn = pInfo->AddressLength * 2;

    if(NULL != pInfo) free(pInfo);

	return iReturn;
}

//////////////////////////////////////////////////////////////////////////
// AdapterExist : Justify whether a adapter with the appointed mac address exist in the host or not
// Args : 
// pMac		- the Hex String of the Mac Address of the adapter which to check up
// Return : 0 ~ Exist; -8 ~ NonExist;
//////////////////////////////////////////////////////////////////////////
INT __stdcall AdapterExist(LPCTSTR pMac)
{
	ULONG ulSize=0;
	INT iMacLen = 0, iReturn = MYERROR_NODEFINE;
	BYTE address[MAX_ADAPTER_ADDRESS_LENGTH];
	PIP_ADAPTER_INFO pInfo = NULL;

	if(pMac == NULL) return MYERROR_INVALID_PARA;

	iMacLen = lstrlen(pMac) >> 1;

	if(iMacLen > MAX_ADAPTER_ADDRESS_LENGTH) return MYERROR_FALSE;
	
	do 
	{
		GetAdaptersInfo(NULL, &ulSize); // Get the AdapterInfo Size

		if(ulSize <= 0) break;
		
		pInfo = (PIP_ADAPTER_INFO)malloc(ulSize);
		
		if(pInfo == NULL) break;
		
		if(GetAdaptersInfo(pInfo, &ulSize) != ERROR_SUCCESS) break;
		
		if(HexStr2Blob(pMac, iMacLen * 2, address, sizeof(address)) != MYERROR_SUCCESS) break;
		
		while (pInfo)
		{
			if(iMacLen == (int)pInfo->AddressLength && memcmp(address, pInfo->Address, iMacLen) == 0)
			{
				iReturn = MYERROR_SUCCESS;
				break;
			}
			pInfo = pInfo->Next;
		}

		if(MYERROR_SUCCESS != iReturn) iReturn = MYERROR_FALSE;

	} while (FALSE);

    if(NULL != pInfo)
	{
		free(pInfo);
		pInfo = NULL;
	}
	
	return iReturn;
}
