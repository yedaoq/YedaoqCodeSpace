#pragma once

#include "DBRecord.h"
#include <vector>

namespace NSDBModule
{

class CDBRecordComparison : public IComparison<CDBRecordBase>
{
public:

	int operator()(const CDBRecordBase& lhs, const CDBRecordBase& rhs)
	{
		if(lhs.GetFieldCount() != rhs.GetFieldCount())
		{
			return false;
		}

		ASSERT(KeyFields_.size());

		for(int i = 0; i < KeyFields_.size(); ++i)
		{
			_ASSERT(KeyFields_[i] < lhs.GetFieldCount());
			unsigned int field = KeyFields_[i];

			int result = lhs.GetField(field).compare(rhs.GetField(field));

			if(result != 0)
			{
				return result;
			}
		}

		return 0;
	}

	CDBRecordComparison()
		: KeyFields_(0)
	{}

	CDBRecordComparison(const CDBRecordComparison& other)
		: KeyFields_(other.KeyFields_)
	{}

	CDBRecordComparison(std::vector<unsigned int> keyFields)
		: KeyFields_(keyFields)
	{}

	CDBRecordComparison(int keyFieldCount, ...)
		:  KeyFields_(0)
	{
		KeyFields_.reserve(keyFieldCount);

		va_list fields;
		va_start(fields, keyFieldCount);

		while(keyFieldCount-- > 0)
		{
			KeyFields_.push_back(va_arg(fields,unsigned int));
		}
	}

	CDBRecordComparison& operator=(const CDBRecordComparison& other)
	{
		KeyFields_.swap(std::vector<unsigned int>(other.KeyFields_));
		return *this;
	}

protected:
	std::vector<unsigned int> KeyFields_;
};


class CDBRecordLessThan : public ILessThan<CDBRecordBase>
{
public:
	bool operator(const CDBRecordBase& lhs, const CDBRecordBase& rhs)
	{
		return Comparison_(lhs, rhs) < 0;
	}

	CDBRecordComparison Comparison_;
};

class CDBRecordGreaterThan
{
public:
	bool operator(const CDBRecordBase& lhs, const CDBRecordBase& rhs)
	{
		return Comparison_(lhs, rhs) > 0;
	}

	CDBRecordComparison Comparison_;
};

class CDBRecordEqual
{
public:
	bool operator(const CDBRecordBase& lhs, const CDBRecordBase& rhs)
	{
		return 0 == Comparison_(lhs, rhs);
	}

	CDBRecordComparison Comparison_;
};

}