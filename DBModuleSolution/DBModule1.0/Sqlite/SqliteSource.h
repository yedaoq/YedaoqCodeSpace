#pragma once

#include "..\DBInterface\DBSourceManager.h"
#include "..\DBInterface\DBSourcePath.h"

namespace NSDBModule
{
	class CSqliteConnection : public IDBConnection
	{
	public:
		CSqliteConnection();
		CSqliteConnection(const tstring& file);

		virtual PropertyEnumerator* EnumPropertyName() = 0;
		virtual const tstring&		GetProperty(const tstring& name) = 0;
		virtual bool				SetProperty(const tstring& name, const tstring& val) = 0;

		virtual int					Open() = 0;
		virtual int					Close() = 0;

		virtual tstring				ToString() = 0;

	protected:
		DBConnectionProperty		Path_;
	};

	class CSqliteSourceManager : IDBSourceManager
	{
	public:
		virtual bool				SourceEnumerable() { return false; };
		virtual SourceEnumerator*	EnumDBSource() { return 0; };

		virtual IDBConnection*		PromptSelectDBSource(IDBConnection* pDefault);
		virtual IDBConnection*		PromptCreateDBSource(IDBConnection* pDefault);

		virtual IDBConnection*		ParseDBConnection(const tchar* pcConnStr);

		//virtual IDBDataAdapter*		OpenDBSource(IDBConnection* pPath, IDBDataAdapter** ppAdapter, IDBFactory** ppFactory) = 0;
	};

}