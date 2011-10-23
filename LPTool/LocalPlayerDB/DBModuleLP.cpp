#include "StdAfx.h"
#include "DBModuleLP.h"
#include "Module/Schema/DBSchemaCppMicro.h"
#include "Module/Schema/BuildInSchemaSerializer.h"
#include <memory>
#include "DBNameMappingLP.h"
#include "DBInterface/DBSourcePath.h"
#include "DBInterface/DBDataAdapter.h"
#include "DBInterface/DBFactory.h"
#include "Sqlite/SqliteSource.h"


using namespace NSDBModule;

int	CDBModuleLP::InitializeBuildinSchema()
{
	CBuildInSchemaSerializer serializer; 
	CBuildInSchemaSerializer::FileRowCollection rows;

	Clear(true);	

	//read file
	CStdioFile file(TEXT("LocalPlayer.dbschema"), CStdioFile::modeRead);
	CString line;
	while(file.ReadString(line))
	{
		rows.push_back((LPCTSTR)line);
	}
	file.Close();

	serializer.Read(make_iterator_enumerator(rows.begin(), rows.end()), *this);

	return 1;
}

CDBModuleLP DBModule;

bool DBInitialize()
{
	CSqliteSourceManager smr;
	std::auto_ptr<IDBConnection> pConn(smr.PromptSelectDBSource(0));
	IDBDataAdapter* PAdapter;
	IDBFactory*		pFactory;
	IDBNameMapping* pMapping = new CDBNameMappingLP();

	if(pConn.get() && smr.OpenDBConnection(pConn.get(), &PAdapter, &pFactory))
	{
		DBModule.AttachToDatabase(PAdapter, pFactory, pMapping);
		return true;
	}

	return false;
}
