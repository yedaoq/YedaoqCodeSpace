#pragma once

#include "mytype.h"
#include <vector>
#include <crtdefs.h>

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{

interface IDBRecord
{
	virtual const tstring&	GetField(unsigned int) const = 0;
	virtual int				SetField(unsigned int, const tstring& val) = 0;
	virtual index_t			GetFieldCount() const = 0;
};

class CDBRecordBase : public IDBRecord
{
protected:
	std::vector<tstring> Fields_;
	unsigned int FieldCount_;

public:
	CDBRecordBase() : Fields_(0), FieldCount_(0) {}

	CDBRecordBase(index_t fieldCount) 
		: Fields_(fieldCount), FieldCount_(fieldCount) { }

	CDBRecordBase(const CDBRecordBase& other) 
		: Fields_(other.Fields_.begin(),other.Fields_.end()), FieldCount_(other.FieldCount_) {}

	CDBRecordBase& operator=(const CDBRecordBase& other)
	{
		this->Fields_.swap(std::vector<tstring>(other.Fields_));
		FieldCount_ = other.FieldCount_;
		return *this;
	}

	virtual const tstring&	GetField(index_t idx) const { return Fields_[idx];	}
	virtual int				SetField(index_t idx, const tstring& val) { Fields_[idx] = val; return 1; }
	virtual index_t			GetFieldCount() const { return FieldCount_; }
};

class CDBRecordAuto : public CDBRecordBase
{
public:
	CDBRecordAuto(int fieldCount = 0)
		: CDBRecordBase(fieldCount)
	{}

	CDBRecordAuto(const CDBRecordAuto& other)
		: CDBRecordBase(other)
	{}

	CDBRecordAuto(const CDBRecordBase& other)
		: CDBRecordBase(other)
	{}

	CDBRecordAuto& operator=(const CDBRecordAuto& other)
	{
		CDBRecordBase::operator=(other);
		return *this;
	}

	CDBRecordAuto& operator=(const CDBRecordBase& other)
	{
		CDBRecordBase::operator=(other);
		return *this;
	}

	virtual index_t GetFieldCount() const { return FieldCount_; }

	virtual int SetField(unsigned int idx, const tstring& val)
	{
		if(idx >= FieldCount_)
		{
			Fields_.resize(idx, tstring());
		}
		Fields_[idx] = val;

		return 1;
	}

	virtual const tstring& GetField(unsigned int idx) const
	{
		static tstring empty;
		if(idx >= FieldCount_)
		{
			return empty;
		}
		return Fields_[idx];
	}
};

}