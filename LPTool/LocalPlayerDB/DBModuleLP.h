#pragma once

#include "mytype.h"
#include "Module\DBModule.h"
#include "Module\Schema\DBSchemaHeaderMicro.h"

class CDBModuleLP : public NSDBModule::CDBModule
{
public:
	class CDBModuleLP()
	{
		Initialize();
	}

	virtual int	InitializeBuildinSchema(); //// buildin schema initialize
	
	#include "LocalPlayer.dbschema"
};

bool DBInitialize();

extern CDBModuleLP DBModule;