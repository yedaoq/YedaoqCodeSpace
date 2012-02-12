#pragma once

#include <Iphlpapi.h>
#include "Enumerator.h"

class CNetworkAdapterManager
{
public:

	static IEnumerator<IP_ADAPTER_INFO>* Enum();
};
