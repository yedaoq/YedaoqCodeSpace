#pragma once

#include "DBRecord.h"
#include <vector>
#include <stdarg.h>
#include "Comaprison.h"

namespace NSDBModule
{

class CDBRecordComparison : public IComparison<IDBRecord>
{
public:

	int operator()(const IDBRecord& lhs, const IDBRecord& rhs) const
	{
		if(lhs.GetFieldCount() != rhs.GetFieldCount())
		{
			return false;
		}

		_ASSERT(KeyFields_.size());

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

	const std::vector<index_t> KeyFields() const { return KeyFields_; }

protected:
	std::vector<index_t> KeyFields_;
};

class CDBRecordLessThan : public IBoolComparison<IDBRecord>
{
public:
	bool operator()(const IDBRecord& lhs, const IDBRecord& rhs) const
	{
		return Comparison(lhs, rhs) < 0;
	}

	//CDBRecordLessThan(){}

	CDBRecordLessThan(const CDBRecordComparison& cmp)
		: Comparison(cmp)
	{}

	CDBRecordLessThan(const CDBRecordLessThan& other)
		: Comparison(other.Comparison)
	{}

	const CDBRecordComparison& Comparison;

protected:
	CDBRecordLessThan& operator=(const CDBRecordLessThan&);
};

class CDBRecordGreaterThan : public IBoolComparison<IDBRecord>
{
public:
	bool operator()(const IDBRecord& lhs, const IDBRecord& rhs)
	{
		return Comparison(lhs, rhs) > 0;
	}

	//CDBRecordGreaterThan(){}

	CDBRecordGreaterThan(const CDBRecordComparison& cmp)
		: Comparison(cmp)
	{}

	CDBRecordGreaterThan(const CDBRecordGreaterThan& other)
		: Comparison(other.Comparison)
	{}

	const CDBRecordComparison& Comparison;

protected:
	CDBRecordGreaterThan& operator=(const CDBRecordGreaterThan&);
};

class CDBRecordEqual : public IBoolComparison<IDBRecord>
{
public:
	bool operator()(const IDBRecord& lhs, const IDBRecord& rhs)
	{
		return 0 == Comparison(lhs, rhs);
	}

	//CDBRecordEqual(){}

	CDBRecordEqual(const CDBRecordComparison& cmp)
		: Comparison(cmp)
	{}

	CDBRecordEqual(const CDBRecordEqual& other)
		: Comparison(other.Comparison)
	{}

	const CDBRecordComparison& Comparison;

protected:
	CDBRecordEqual& operator=(const CDBRecordEqual&);
};

}