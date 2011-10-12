#pragma once

#include "mytype.h"
#include <vector>
#include <crtdefs.h>
#include "DBRecordComparison.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	class CDBRecordToField
	{
	public:
		CDBRecordToField()
			: Field_(-1)
		{}

		CDBRecordToField(int field)
			: Field_(field)
		{}

		const tstring& operator()(const CDBRecordBase& rec) 
		{ 
			_ASSERT(Field_ != -1);
			return rec.GetField(Field_); 
		}

		int Field_;
	};

	class CDBRecordFilter
	{
	public:
		CDBRecordFilter(){}

		CDBRecordFilter(const CDBRecordBase& rec, const CDBRecordComparison& cmp)
			: Key(rec), Comparison(cmp)
		{}

		bool operator()(const CDBRecordBase& rec)
		{ 
			//int result = Comparison(rec, Key);
			return 0 == Comparison(rec, Key); 
		}

		CDBRecordComparison Comparison;
		CDBRecordAuto		Key;
	};

	inline bool DBRecordAssign(IDBRecord& des, const IDBRecord& src)
	{
		_ASSERT(des.GetFieldCount() == src.GetFieldCount());
		for(int i = 0; i < min(des.GetFieldCount(), src.GetFieldCount()); ++i)
		{
			des.SetField(i, src.GetField(i));
		}

		return true;
	}
}