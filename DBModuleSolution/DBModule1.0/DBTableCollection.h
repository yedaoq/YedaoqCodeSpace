#pragma once

#include <memory>
#include <vector>
#include <atlcomcli.h>
#include "mytype.h"
#include "Enumerator.h"

namespace NSDBModule
{
	class CDBTable;
	class CDBTableSchema;
	class CDBModule;

	typedef std::tr1::shared_ptr<CDBTable>							DBTablePtr;
	typedef std::tr1::shared_ptr<const CDBTable>					DBTablePtr_const;
	typedef std::vector<ATL::CAdapt<DBTablePtr>>					DBTableCollection;
	typedef CIteratorEnumerator<DBTableCollection::const_iterator>	DBTableEnumerator;

	class CDBTableCollection
	{
	public:

		int						Clear(bool bBuildIn = false);

		DBTableEnumerator		Enum() const;

		DBTablePtr				Append(const tstring& name, CDBModule* module, bool bBuildIn = false);
		DBTablePtr				Append(const CDBTableSchema& schema, CDBModule* module, bool bBuildIn = false);

		DBTablePtr				Remove(const tstring& name);
		DBTablePtr				Remove(index_t tbl);

		DBTablePtr_const operator[](const tstring& tbl) const;
		DBTablePtr_const operator[](index_t tbl) const;

		DBTablePtr operator[](const tstring& tbl) { return std::tr1::const_pointer_cast<CDBTable,const CDBTable>(const_cast<const CDBTableCollection&>(*this)[tbl]); }
		DBTablePtr operator[](index_t tbl) { return std::tr1::const_pointer_cast<CDBTable, const CDBTable>(const_cast<const CDBTableCollection&>(*this)[tbl]); }

	protected:
		DBTableCollection Tables;
	};
}

