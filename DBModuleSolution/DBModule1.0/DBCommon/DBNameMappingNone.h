#pragma once

#include "..\DBInterface\DBNameMapping.h"

namespace NSDBModule
{
	class CDBNameMappingNone : public IDBNameMapping
	{
	public:
		virtual tstring FromDBName(const tstring& dbName, const ContextDBNameMapping& context)
		{
			return dbName;
		}

		virtual tstring ToDBName(const tstring& name, const ContextDBNameMapping& context)
		{
			return name;
		}
	};
}