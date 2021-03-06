#pragma once

#include "mytype.h"
#include "DBCommand.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	enum EnumDBNameCategory { Table, Column };
	class CDBTableSchema;

	struct ContextDBNameMapping
	{
		EnumDBNameCategory		Category;
		const CDBTableSchema*	Table;
	};

	interface IDBNameMapping
	{
		virtual ~IDBNameMapping() = 0 {}

		virtual tstring FromDBName(const tstring& dbName, const ContextDBNameMapping& context) = 0;
		virtual tstring ToDBName(const tstring& name, const ContextDBNameMapping& context) = 0;
	};

}