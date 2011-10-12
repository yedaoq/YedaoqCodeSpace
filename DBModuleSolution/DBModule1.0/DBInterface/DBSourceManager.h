#pragma once

#include "mytype.h"
#include "Enumerator.h"
#include "DBDataAdapter.h"
#include "DBFactory.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface IDBConnection;

	interface IDBSourceManager
	{
		typedef IEnumerator<IDBConnection> SourceEnumerator;

		virtual ~IDBSourceManager() {}

		virtual const tstring&		ToString() = 0;

		virtual SourceEnumerator*	EnumDBSource() = 0;

		virtual IDBConnection*		PromptSelectDBSource(IDBConnection* pDefault) = 0;
		virtual IDBConnection*		PromptCreateDBSource(IDBConnection* pDefault) = 0;

		virtual IDBConnection*		ParseDBConnection(const tchar* pcConnStr) = 0;
		
		virtual bool				OpenDBConnection(IDBConnection* pConn, IDBDataAdapter** ppAdapter, IDBFactory** ppFactory) = 0;
	};
}
