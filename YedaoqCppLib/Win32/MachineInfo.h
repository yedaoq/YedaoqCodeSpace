
#if !defined(_MACHINEINFO_H)
#define _MACHINEINFO_H

#include <Iphlpapi.h>

//////////////////////////////////////////////////////////////////////////
// AdapterDeal : prototype of the function which will be passed to EnumdAdapters as a calllback
// args : 
// pInfo		- the adapter info
// return : return TRUE if you want contiune enumerate and False when not
//////////////////////////////////////////////////////////////////////////
typedef BOOL (*AdapterDeal)(PIP_ADAPTER_INFO pInfo);

INT _stdcall EnumdAdapters(AdapterDeal callback);

INT __stdcall GetHostMacs(LPTSTR pBuffer, INT iLen);

INT __stdcall GetDefaultMac(LPTSTR pBuffer, INT iLen);

INT __stdcall AdapterExist(LPCTSTR pMac);

#endif // !defined(_MACHINEINFO_H)
