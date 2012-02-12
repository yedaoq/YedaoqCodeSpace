#include "StdAfx.h"
#include "NetworkAdapterManager.h"
#include "CNetworkAdapterEnumerator.h"

IEnumerator<IP_ADAPTER_INFO>* CNetworkAdapterManager::Enum()
{
	return new CNetworkAdapterEnumerator();
}
