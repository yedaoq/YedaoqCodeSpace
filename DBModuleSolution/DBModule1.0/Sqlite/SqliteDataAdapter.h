#pragma once

#include "../DBDataAdapter.h"

namespace NSDBModule
{
	class CSqliteDataAdapter : public IDBDataAdapter
	{
	public:
		virtual ~CSqliteDataAdapter() {}

		virtual DBTableEnumPtr		EnumTable() = 0;
		virtual DBColumnEnumPtr		EnumColumn(const tstring& tblName) = 0;

		virtual DBRecordEnumPtr		Select(const CDBRecordBase& condition, const CDBRecordComparison& comparison) = 0;
		virtual DBRecordEnumPtr		Select(const IDBCommand& cmd) = 0;

		virtual int					Execute(const IDBCommand& cmd) = 0;
	};
}