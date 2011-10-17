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
	interface IDBDataAdapter;

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

		//index_t					Index;	// index of table in memory module
		tstring					DBName;	// name of table in database
		tstring					Name;	// name of table in memory module
		flag_t					Flag;	
		ColumnCollection		Columns;
		
	public:
		const DBColumnSchema&	operator[](const tstring& dbCol) const;
		const DBColumnSchema&	operator[](index_t col) const;

		DBColumnSchema& operator[](const tstring& dbCol) { return const_cast<DBColumnSchema&>(const_cast<const CDBTableSchema&>(*this)[dbCol]); }
		DBColumnSchema& operator[](index_t col) { return const_cast<DBColumnSchema&>(const_cast<const CDBTableSchema&>(*this)[col]);	}

		CDBTableSchema()
			: Columns(0), Flag(None)
		{}

		CDBTableSchema(const tstring& dbName, const tstring& name)
			: DBName(dbName), Name(name), Columns(0), Flag(None)
		{}

		CDBTableSchema(const CDBTableSchema& other)
			: DBName(other.DBName), Name(other.Name), Columns(other.Columns), Flag(other.Flag)
		{}

		CDBTableSchema& operator=(const CDBTableSchema& other)
		{
			DBName = other.DBName;
			Name = other.Name;
			Flag = other.Flag;
			Columns.swap(ColumnCollection(other.Columns));

			return *this;
		}

		bool IsBuildin() const { return static_cast<bool>(Flag & BuildIn); }
		bool IsDBExist() const { return static_cast<bool>(Flag & DBExist); }

		IEnumerator<DBColumnSchema>* EnumColumn() const { return new_iterator_enumerator(Columns.begin(), Columns.end()); }

		bool FindByName(const tstring& col, DBColumnSchema& colSchema) const;
		bool FindByIndex(index_t col, DBColumnSchema& colSchema) const;
		
		bool ModifyColumn(const DBColumnSchema& col);
		bool AppendColumn(DBColumnSchema& col);
		bool RemoveColumn(index_t col);

		void SetFlag(flag_t flag, bool bAppend)
		{
			if(bAppend)
			{
				Flag |= flag;
			}
			else
			{
				Flag &= ~flag;
			}
		}

		void Clear(bool bBuildIn = false);
		bool Load(IDBDataAdapter* dbAdapter);
	};
}
