#pragma once

#include "../DBCommon/DBDataTypeProviderBase.h"
#include "Singlton.h"

namespace NSDBModule
{
	class CSqliteDataTypeProvider : public CDBDataTypeProvider, public CSingleton<CSqliteDataTypeProvider>
	{
	public:
		CSqliteDataTypeProvider();
	};
}