#pragma once

#include "DBDataType.h"

namespace NSDBModule
{
	class CDBDataTypeProvider : public IDBDataTypeProvider
	{
	public:

		typedef std::vector<IDBDataType*> DBDataTypeVct;
		typedef CIteratorEnumerator<DBDataTypeVct::iterator> DBDataTypeEnumerator;

		CDBDataTypeProvider()
			: DataTypes(CPPDATATYPECOUNT)
		{}

		virtual IDBDataType*		ParseDBTypeStr(tstring type);
		virtual IDBDataType*		GetPreferredDBType(EnumCppDataType type);

		virtual bool				DBTypeEnumerable() { return true; };
		virtual DBDataTypeEnumerator GetEnumerator();

		virtual int					RegisterDataType(const IDBDataType& type);

	protected:
		DBDataTypeVct				DataTypes;
	};
}
