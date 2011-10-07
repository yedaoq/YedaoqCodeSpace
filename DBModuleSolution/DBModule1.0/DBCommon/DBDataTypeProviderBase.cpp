#include "DBDataTypeProviderBase.h"

using namespace NSDBModule;

IDBDataType* CDBDataTypeProvider::ParseDBTypeStr(const tstring& type)
{
	for(DBDataTypeVct::iterator iter = DataTypes.begin(); iter != DataTypes.end(); ++iter)
	{
		if((*iter)->ToString() == type)
		{
			return *iter;
		}
	}

	for (DBDataTypeParserVct::iterator iter = Parsers.begin(); iter != Parsers.end(); ++iter)
	{
		IDBDataType* dbType = 0;
		if(dbType = (*iter)->Parse(type))
		{
			RegisterDataType(dbType);
			return dbType;
		}
	}
	
	return 0;
}

IDBDataType* CDBDataTypeProvider::GetPreferredDBType(EnumCppDataType type)
{
	for(DBDataTypeVct::iterator iter = DataTypes.begin(); iter != DataTypes.end(); ++iter)
	{
		if((*iter)->PreferredCppDataType() == type)
		{
			return *iter;
		}
	}

	for(DBDataTypeVct::iterator iter = DataTypes.begin(); iter != DataTypes.end(); ++iter)
	{
		if((*iter)->CompatibleWith(type))
		{
			return *iter;
		}
	}

	return 0;
}

IDBDataTypeProvider::DBDataTypeEnumPtr CDBDataTypeProvider::GetEnumerator()
{
	return DBDataTypeEnumPtr(new_iterator_enumerator(DataTypes.begin(), DataTypes.end()));
}

int	CDBDataTypeProvider::RegisterDataType(IDBDataType* type)
{
	//DataTypes.push_back(static_cast<IDBDataType*>(type.Clone()));
	DataTypes.push_back(type);
	return 1;
}

int	CDBDataTypeProvider::RegisterParser(IDBDataTypeParser* parser)
{
	Parsers.push_back(parser);
	return 1;
}

int	CDBDataTypeProvider::Clear()
{
	for (DBDataTypeVct::iterator iter = DataTypes.begin(); iter != DataTypes.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	for (DBDataTypeParserVct::iterator iter = Parsers.begin(); iter != Parsers.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	DataTypes.clear();
	Parsers.clear();

	return 1;
}