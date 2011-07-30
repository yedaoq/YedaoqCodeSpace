#pragma once

#include "DBRecord.h"
#include "ColumnSchema.h"
#include "Enumerator.h"

#include <set>
#include <vector>
#include <crtdbg.h>

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{

typedef std::vector<CDBRecordBase>	RecordVector;
typedef std::vector<ColumnSchema>	ColumnSchemaVector;
typedef std::auto_ptr<IEnumerator<CDBRecordBase>> RecEnumPtr;

interface IDBCollection
{
public:
	virtual bool Loaded() const = 0;
	virtual int Load() = 0;

	virtual RecEnumPtr GetEnumerator() = 0;
	virtual int GetRecords(const CDBRecordBase& rec, RecordVector& ve) = 0;
	virtual RecPtr NewRecord() = 0;

	virtual const std::vector<ColumnSchema>& GetColumns() = 0;

	virtual bool Exist(const CDBRecordBase& rec, bool matchAllFields) = 0;

	virtual int Update(const CDBRecordBase& origin,const CDBRecordBase& current) = 0;
	virtual int Insert(const CDBRecordBase& rec) = 0;
	virtual int Delete(const CDBRecordBase& rec) = 0;	
};

class CDBCollectionBase : public IDBCollection
{
public:

	virtual bool Loaded() const { return FlagLoad_; }
	virtual RecPtr NewRecord(){ return RecPtr(new CDBRecordBase(GetColumns().size())); }
	virtual const ColumnSchemaVector& GetColumns() { return Columns_; }

protected:
	CDBCollectionBase(void)  : Columns_(0), FlagLoad_(false) {};
	CDBCollectionBase(const CDBCollectionBase& other);
	CDBCollectionBase& operator =(const CDBCollectionBase& other);

	std::vector<ColumnSchema> Columns_;
	bool FlagLoad_;
};

template<typename iter_t>
class CDBColumnEnumerator : public IEnumerator<tstring>
{
public:
	CDBColumnEnumerator(const iter_t begin, const iter_t end, int field)
		: first_(begin), last_(end), field_(field), iter_(begin)
	{}

	virtual bool MoveNext(tstring& obj)
	{
		if(iter_ != last_)
		{
			obj = iter_->GetField(field_);
			++iter_;
			return true;
		}
		return false;
	}

	virtual const tstring& Current()
	{
		return iter_->GetField(field_);
	}

	virtual void Reset()
	{
		iter_ = first_;
	}

protected:
	iter_t first_;
	iter_t last_;
	iter_t iter_;

	int field_;
};

template<typename iter_t>
class CRecordSetEnumerator : public IEnumerator<CDBRecordBase>
{
public:
	CRecordSetEnumerator(){}

	CRecordSetEnumerator(iter_t begin, iter_t end)
		:first_(begin), last_(end), iter_(begin)
	{}

	CRecordSetEnumerator(const CRecordSetEnumerator& other)
	{
		first_ = other.first_;
		last_ = other.last_;
		iter_ = other.iter_;
	}

	CRecordSetEnumerator& operator=(const CRecordSetEnumerator& other)
	{
		first_ = other.first_;
		last_ = other.last_;
		iter_ = other.iter_;

		return *this;
	}

	virtual bool MoveNext()
	{
		iter_t tmp = iter_;

		if(++tmp != last_)
		{
			iter_ = tmp;
			return true;
		}
		return false;
	}

	virtual bool MoveNext(CDBRecordBase& obj)
	{
		if(MoveNext())
		{
			obj = Current();
			return true;
		}
		return false;
	}

	virtual const CDBRecordBase& Current()
	{
		_ASSERT(iter_ != last_);
		return static_cast<CDBRecordBase&>(*(iter_));
	}

	virtual void Reset()
	{
		iter_ = first_;
	}

protected:
	iter_t first_;
	iter_t last_;
	iter_t iter_;
};

}