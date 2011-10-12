#pragma once

#include "..\DBInterface\DBDataType.h"
#include "mytype.h"
#include <vector>

namespace NSDBModule
{
	class CDBDataTypeProvider : public IDBDataTypeProvider
	{
	public:
		typedef std::vector<IDBDataType*>			DBDataTypeVct;
		typedef std::vector<IDBDataTypeParser*>	DBDataTypeParserVct;

	public:
		virtual ~CDBDataTypeProvider() { Clear(); }

		CDBDataTypeProvider()
			: DataTypes(0)
		{}

		virtual IDBDataType*		ParseDBTypeStr(const tstring& type);
		virtual IDBDataType*		GetPreferredDBType(EnumCppDataType type);

		virtual bool				DBTypeEnumerable() { return true; };
		virtual DBDataTypeEnumPtr	GetEnumerator();

		virtual int					RegisterDataType(IDBDataType* type);
		virtual int					RegisterParser(IDBDataTypeParser* parser);

		virtual int					Clear();

	protected:
		DBDataTypeVct				DataTypes;
		DBDataTypeParserVct			Parsers;
	};
}
