#pragma once

#include "DBInterface\DBSourceManager.h"
#include "DBInterface\DBSourcePath.h"
#include <vector>

namespace NSDBModule
{
	class CSqliteConnection : public IDBConnection
	{
	public:
		typedef std::vector<DBConnectionProperty> PropertyCollection;

	public:
		CSqliteConnection();
		CSqliteConnection(const tstring& file);

		virtual PropertyEnumerator* EnumProperty();
		virtual const tstring&		GetProperty(const tstring& name);
		virtual bool				SetProperty(const tstring& name, const tstring& val);

		//virtual int					Open();
		//virtual int					Close();

		virtual tstring				ToString();

	protected:
		PropertyCollection			Properties_;
	};

	class CSqliteSourceManager : public IDBSourceManager
	{
	public:

		virtual const tstring&		ToString();

		virtual SourceEnumerator*	EnumDBSource() { return 0; };

		virtual IDBConnection*		PromptSelectDBSource(IDBConnection* pDefault);
		virtual IDBConnection*		PromptCreateDBSource(IDBConnection* pDefault);

		virtual IDBConnection*		ParseDBConnection(const tchar* pcConnStr);

		virtual bool				OpenDBConnection(IDBConnection* pConn, IDBDataAdapter** ppAdapter, IDBFactory** ppFactory);
	};

}