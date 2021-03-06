#pragma once

#include "mytype.h"
#include "Enumerator.h"
#include <vector>

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	class	CDBModule;
	class	CDBTableSchema;
	struct	DBColumnSchema;
	struct	BuildInSchemaSerializeObject;

	class CBuildInSchemaSerializer
	{
	public:
		typedef std::vector<tstring> FileRowCollection;

	public:
		CBuildInSchemaSerializer();

		bool Read(IEnumerator<tstring>& rows, CDBModule& module);
		bool Write(CDBModule& module, FileRowCollection& rows);

	protected:
		bool ReadAllTable(IEnumerator<tstring>& rows, CDBModule& module);
		bool ReadTable(IEnumerator<tstring>& rows, CDBTableSchema& tbl);

		bool WriteAllTable(CDBModule& module, FileRowCollection& rows);
		bool WriteTable(const CDBTableSchema& tbl, FileRowCollection& rows);

	protected:
		BuildInSchemaSerializeObject* SerializeObject;
		
	};
}

