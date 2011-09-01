#include "SqliteDataTypeProvider.h"

using namespace NSDBModule;

CSqliteDataTypeProvider::CSqliteDataTypeProvider()
{
	RegisterDataType(new CDBDataTypeInteger());
	RegisterDataType(new CDBDataTypeFloat());
	RegisterDataType(new CDBDataTypeDouble());

	RegisterParser(new CDBDataTypeDecimalParser());
	RegisterParser(new CDBDataTypeStringParser());
}