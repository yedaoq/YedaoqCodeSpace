#include "SqliteDataTypeProvider.h"
#include "..\DBCommon\DBDataTypeCommon.h"
#include "..\DBCommon\DBDataTypeParser.h"

using namespace NSDBModule;

CSqliteDataTypeProvider::CSqliteDataTypeProvider()
{
	RegisterDataType(new CDBDataTypeInteger());
	RegisterDataType(new CDBDataTypeFloat());
	RegisterDataType(new CDBDataTypeDouble());
	RegisterDataType(new CDBDataTypeUnknow());

	RegisterParser(new CDBDataTypeDecimalParser());
	RegisterParser(new CDBDataTypeStringParser());
}