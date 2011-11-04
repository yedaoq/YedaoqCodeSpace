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

	typedef CDBTable*						DBTablePtr;
	typedef std::vector<DBTablePtr>			DBTableCollection;
	typedef IEnumerator<DBTablePtr>			DBTableEnumerator;

	class CDBTableCollection
	{
	public:

		int						Count() const { return Tables.size(); }
		int						Clear(bool bBuildIn = false);

		DBTableEnumerator*		Enum() const;
		IEnumerator<tstring>*	EnumName() const;

		CDBTable*				Append(const tstring& name, CDBModule* module, bool bBuildIn = false);
		CDBTable*				Append(const CDBTableSchema& schema, CDBModule* module, bool bBuildIn = false);

		int						Remove(const tstring& name);
		int						Remove(index_t tbl);

		int						IndexOf(const tstring& name);

		const CDBTable* operator[](const tstring& tbl) const;
		const CDBTable* operator[](index_t tbl) const;

		CDBTable* operator[](const tstring& tbl) 
		{ 
			return const_cast<CDBTable*>(const_cast<const CDBTableCollection&>(*this)[tbl]); 
		}

		CDBTable* operator[](index_t tbl) 
		{ 
			return const_cast<CDBTable*>(const_cast<const CDBTableCollection&>(*this)[tbl]); 
		}

	protected:
		
		void ReleaseTables(DBTableCollection::iterator begin, DBTableCollection::iterator end);

	protected:
		DBTableCollection Tables;
	};
}

