#pragma once

#include "mytype.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	class CDBModule;
	class CDBTable;
	//class CDBTableSchema;
	//struct DBColumnSchema;
	//interface IDBDataAdapter;
	//interface IDBNameMapping;
	//class CDBTableCollection;

	class CDBSchemaLoader
	{
	public:
		CDBSchemaLoader(CDBModule* module)
			: DBModule(module)
		{}

		bool Load();

		bool LoadAllTable();
		bool LoadTable(const tstring& dbName, CDBTable* pTbl);

	protected:
		CDBModule*		DBModule;
	};
}

