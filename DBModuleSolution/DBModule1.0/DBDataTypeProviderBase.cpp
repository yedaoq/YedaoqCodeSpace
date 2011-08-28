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

	return 0;
}

IDBDataTypeProvider::DBDataTypeEnumPtr CDBDataTypeProvider::GetEnumerator()
{
	return DBDataTypeEnumPtr(new_iterator_enumerator(DataTypes.begin(), DataTypes.end()));
}

int	CDBDataTypeProvider::RegisterDataType(const IDBDataType& type)
{
	DataTypes.push_back(static_cast<IDBDataType*>(type.Clone()));

	return 1;
}
int	CDBDataTypeProvider::Clear()
{
	for (DBDataTypeVct::iterator iter = DataTypes.begin(); iter != DataTypes.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	DataTypes.clear();

	return 1;
}