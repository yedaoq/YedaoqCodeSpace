#pragma once

#include "DBInterface/DBNameMapping.h"
#include "Singlton.h"

class CSqliteNameMapping : public NSDBModule::IDBNameMapping, public CSingleton<CSqliteNameMapping>
{
public:
	virtual tstring FromDBName(const tstring& dbName, const NSDBModule::ContextDBNameMapping& context);
	virtual tstring ToDBName(const tstring& name, const NSDBModule::ContextDBNameMapping& context);
};
