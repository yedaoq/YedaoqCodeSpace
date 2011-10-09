#include "SqliteFactory.h"
#include "..\DBInterface\DBCommand.h"
#include "..\DBInterface\DBCommandBuilder.h"
#include "..\DBInterface\DBDataAdapter.h"
#include "..\DBInterface\DBDataType.h"
#include "..\DBInterface\DBNameMapping.h"
#include "..\DBCommon\DBCommandBase.h"
#include "..\DBCommon\DBCommandBuilderBase.h"
#include "SqliteDataAdapter.h"
#include "SqliteDataTypeProvider.h"

using namespace NSDBModule;

IDBCommand*	CSqliteFactory::CreateDBCommand()
{
	return new CDBCommandBase;
}

IDBCommandBuilder* CSqliteFactory::CreateDBCommandBuilder()
{
	return new CDBCommandBuilderBase;
}

IDBDataTypeProvider* CSqliteFactory::CreateDBDataTypeProvider()
{
	return new CSqliteDataTypeProvider;
}