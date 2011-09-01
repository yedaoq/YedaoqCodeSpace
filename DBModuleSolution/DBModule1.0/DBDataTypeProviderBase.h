#pragma once

#include "DBDataType.h"
#include "mytype.h"

namespace NSDBModule
{
	class CDBDataTypeProvider : public IDBDataTypeProvider
	{
	public:
		typedef std::vector<const IDBDataType*>		DBDataTypeVct;
		typedef std::vector<IDBDataTypeParser*> DBDataTypeParserVct;

	public:
		virtual ~CDBDataTypeProvider() { Clear(); }

		CDBDataTypeProvider()
			: DataTypes(CPPDATATYPECOUNT)
		{}

		virtual IDBDataType*		ParseDBTypeStr(const tstring& type);
		virtual IDBDataType*		GetPreferredDBType(EnumCppDataType type);

		virtual bool				DBTypeEnumerable() { return true; };
		virtual DBDataTypeEnumPtr	GetEnumerator();

		virtual int					RegisterDataType(const IDBDataType& type);
		virtual int					RegisterParser(IDBDataAdapter& parser);

		virtual int					Clear();

	protected:
		DBDataTypeVct				DataTypes;
		DBDataTypeParserVct			Parsers;
	};
}
