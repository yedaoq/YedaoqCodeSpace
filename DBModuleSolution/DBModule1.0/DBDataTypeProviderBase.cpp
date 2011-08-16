#include "DBDataTypeProviderBase.h"

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

IEnumerator<IDBDataType*> CDBDataTypeProvider::GetEnumerator()
{
	return make_iterator_enumerator(DataTypes.begin(), DataTypes.end());
}

int	CDBDataTypeProvider::RegisterDataType(const IDBDataType& type)
{
	DataTypes.push_back(type.Clone());

	return 1;
}
