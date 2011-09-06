#pragma once

#include "DBDataType.h"
#include "mytype.h"

namespace NSDBModule
{
	

	class CDBDataTypeProvider : public IDBDataTypeProvider
	{
	public:
		typedef std::vector<const IDBDataType*>			DBDataTypeVct;
		typedef std::vector<const IDBDataTypeParser*>	DBDataTypeParserVct;

	public:
		virtual ~CDBDataTypeProvider() { Clear(); }

		CDBDataTypeProvider()
			: DataTypes(CPPDATATYPECOUNT)
		{}

		virtual const IDBDataType*	ParseDBTypeStr(const tstring& type);
		virtual const IDBDataType*	GetPreferredDBType(EnumCppDataType type);

		virtual bool				DBTypeEnumerable() { return true; };
		virtual DBDataTypeEnumPtr	GetEnumerator();

		virtual int					RegisterDataType(const IDBDataType* type);
		virtual int					RegisterParser(const IDBDataTypeParser* parser);

		virtual int					Clear();

	protected:
		DBDataTypeVct				DataTypes;
		DBDataTypeParserVct			Parsers;
	};
}
