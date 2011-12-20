#pragma once

#include "mytype.h"
#include "Module\DBModule.h"
#include "Module\Schema\DBSchemaHeaderMicro.h"
#include "Singlton.h"

class CDBModuleLP : public NSDBModule::CDBModule, public CSingleton<CDBModuleLP>
{
public:
	class CDBModuleLP()
	{
		Initialize();
	}

	virtual int	InitializeBuildinSchema(); //// buildin schema initialize
};

bool DBInitialize();

//extern CDBModuleLP g_DBModule;