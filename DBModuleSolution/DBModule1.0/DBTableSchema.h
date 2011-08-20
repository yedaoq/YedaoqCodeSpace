#pragma once
#include "DBColumnSchema.h"
#include "mytype.h"
#include <vector>
#include <memory>
#include "Enumerator.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	class CDBTableSchema
	{
	public:
		typedef std::vector<DBColumnSchema>								ColumnCollection;
		typedef CIteratorEnumerator<ColumnCollection::const_iterator>	ColumnEnumerator;
		typedef unsigned int flag_t;

		enum EnumFlag
		{ 
			None = 0, 
			BuildIn,					// indicate a table in build schema
			DBExist,					// indicate a table exist in database
		};

		tstring					DBName;	// name of table in database
		tstring					Name;	// name of table in memory module
		flag_t					Flag;	
		ColumnCollection		Columns;
		
	public:
		DBColumnSchema&			operator[](const tstring& dbCol);
		DBColumnSchema&			operator[](index_t col);

		const DBColumnSchema&	operator[](const tstring& dbCol) const
		{
			return const_cast<CDBTableSchema&>(*this)[dbCol];
		}

		const DBColumnSchema&	operator[](index_t col) const
		{
			return const_cast<CDBTableSchema&>(*this)[col];
		}

		CDBTableSchema(const tstring& dbName, const tstring& name)
			: DBName(dbName), Name(name), Columns(0)
		{}		
		bool IsBuildin() const		{ return Flag & BuildIn; }
		bool IsDBExist() const		{ return Flag & DBExist; }

		ColumnEnumerator EnumColumn() const { return make_iterator_enumerator(Columns.begin(), Columns.end()); }

		bool FindByDBName(const tstring& col, DBColumnSchema& colSchema);
		bool FindByIndex(index_t col, DBColumnSchema& colSchema);
		
		bool SetColumn(const DBColumnSchema& col);
		bool AddColumn(DBColumnSchema& col);

		bool Validate();
		void Clear();
		bool Load(IDBDataAdapter* dbAdapter);
	};
}
