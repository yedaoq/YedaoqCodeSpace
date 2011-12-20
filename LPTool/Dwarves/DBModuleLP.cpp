#include "StdAfx.h"
#include "DBModuleLP.h"
#include "Module/Schema/DBSchemaCppMicro.h"
#include "Module/Schema/BuildInSchemaSerializer.h"
#include <memory>
//#include "DBNameMappingLP.h"
#include "SqliteNameMapping.h"
#include "DBInterface/DBSourcePath.h"
#include "DBInterface/DBDataAdapter.h"
#include "DBInterface/DBFactory.h"
#include "SqliteSource.h"
#include "mytype.h"
#include <fstream>
#include <iosfwd>

using namespace NSDBModule;
using namespace std;

int	CDBModuleLP::InitializeBuildinSchema()
{
	CBuildInSchemaSerializer serializer; 
	CBuildInSchemaSerializer::FileRowCollection rows;
	static const int szBufSize = 1024;
	tchar szBuf[szBufSize];

	Clear(true);	

	//read file
	tifstream file("LocalPlayer.dbschema");
	while(file.getline(szBuf, szBufSize))
	{
		rows.push_back(szBuf);
	}
	file.close();

	serializer.Read(make_iterator_enumerator(rows.begin(), rows.end()), *this);

	return 1;
}

//CDBModuleLP g_DBModule;

//bool DBInitialize()
//{
//	CSqliteSourceManager smr;
//	std::auto_ptr<IDBConnection> pConn(smr.PromptSelectDBSource(0));
//	IDBDataAdapter* PAdapter;
//	IDBFactory*		pFactory;
//	IDBNameMapping* pMapping = new CSqliteNameMapping();
//
//	if(pConn.get() && smr.OpenDBConnection(pConn.get(), &PAdapter, &pFactory))
//	{
//		CDBModuleLP::GetInstance().AttachToDatabase(PAdapter, pFactory, pMapping);
//		return true;
//	}
//
//	return false;
//}
