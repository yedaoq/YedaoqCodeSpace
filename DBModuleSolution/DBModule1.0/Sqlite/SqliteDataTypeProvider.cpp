#include "SqliteDataTypeProvider.h"
#include "..\DBDataTypeCommon.h"

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