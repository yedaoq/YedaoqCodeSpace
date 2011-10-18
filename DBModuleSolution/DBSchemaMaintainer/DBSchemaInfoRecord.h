#pragma once
#include "Module\Schema\DBColumnSchema.h"
#include "Module\DBTableCollection.h"
#include "mytype.h"
#include "Module\DBRecord.h"
#include "Module\DBModule.h"
#include "Module\Schema\DBTableSchema.h"
#include "Module\DBRecordFunction.h"

using namespace NSDBModule;

class CDBTableInfoRecord : public IDBRecord
{
public:
	enum EnumField
	{
		State,
		Name,
		DBName,
		Buildin,
		DBExist,		
	};

public:
	CDBTableInfoRecord()
		: TblPtr(0)
	{}

	CDBTableInfoRecord(DBTablePtr pTbl)
		: TblPtr(pTbl)
	{}

	virtual const tstring& GetField(unsigned int idx) const;
	virtual int	SetField(unsigned int idx, const tstring& val);
	virtual index_t GetFieldCount() const { return 5; }

	void SetTable(DBTablePtr pTbl) { TblPtr = pTbl; }
	bool IsStateNormal() const;

protected:
	DBTablePtr	TblPtr;
};

class CDBColumnInfoRecord : public IDBRecord
{
public:
	enum EnumField
	{
		State,
		Name,
		DBName,
		Buildin,
		DBExist,
		Type,
		DBType,
		KeyCol,
		DBPK,
		DBUnnull,
		RelyTbl,
		RelyCol,
		VisiCol,
		Index,
	};

public:
	CDBColumnInfoRecord()
		: ColPtr(0)
	{}

	CDBColumnInfoRecord(DBColumnSchema* pCol)
		: ColPtr(pCol)
	{}

	virtual const tstring& GetField(unsigned int idx) const;
	virtual int	SetField(unsigned int idx, const tstring& val);
	virtual index_t GetFieldCount() const { return 10; }

	void SetColumn(DBColumnSchema* pCol) { ColPtr = pCol; }
	bool IsStateNormal() const;

protected:
	DBColumnSchema*	ColPtr;
};

class CDBTableInfoEnumerator : public IEnumerator<IDBRecord>
{
public:
	CDBTableInfoEnumerator(CDBModule* pModule)
		: DBModulePtr(pModule), InnerEnumPtr(pModule->Tables().Enum())
	{
		_ASSERT(pModule);
	}

	CDBTableInfoEnumerator(const CDBTableInfoEnumerator& other)
		: DBModulePtr(other.DBModulePtr), InnerEnumPtr(DBModulePtr->Tables().Enum())
	{}

	virtual bool MoveNext()
	{
		return InnerEnumPtr->MoveNext();
	}

	virtual bool MoveNext(IDBRecord& rec)
	{
		if(MoveNext())
		{
			DBRecordAssign(rec, Current());
			return true;
		}

		return false;
	}

	virtual const IDBRecord& Current()
	{
		static CDBTableInfoRecord rec;
		rec.SetTable(InnerEnumPtr->Current());
		return rec;
	}

	virtual void Reset()
	{
		InnerEnumPtr = std::auto_ptr<DBTableEnumerator>(DBModulePtr->Tables().Enum());
	}

	virtual ICloneable* Clone() const
	{
		return new CDBTableInfoEnumerator(*this);
	}

protected:
	CDBModule*						 DBModulePtr;
	std::auto_ptr<DBTableEnumerator> InnerEnumPtr;
};

class CDBColumnInfoEnumerator : public IEnumerator<IDBRecord>
{
public:
	CDBColumnInfoEnumerator(CDBTableSchema* pTbl)
		: DBTableSchemaPtr(pTbl), InnerEnumPtr(0)
	{
		Reset();
	}

	CDBColumnInfoEnumerator(const CDBColumnInfoEnumerator& other)
		: DBTableSchemaPtr(other.DBTableSchemaPtr), InnerEnumPtr(DBTableSchemaPtr->EnumColumn())
	{}

	virtual bool MoveNext()
	{
		if(InnerEnumPtr.get())
		{
			return InnerEnumPtr->MoveNext();
		}
		return false;
	}

	virtual bool MoveNext(IDBRecord& rec)
	{
		if(MoveNext())
		{
			DBRecordAssign(rec, Current());
			return true;
		}

		return false;
	}

	virtual const IDBRecord& Current()
	{
		static CDBColumnInfoRecord rec;
		rec.SetColumn(const_cast<DBColumnSchema*>(&InnerEnumPtr->Current()));
		return rec;
	}

	virtual void Reset()
	{
		if(DBTableSchemaPtr)
		{
			InnerEnumPtr = std::auto_ptr<IEnumerator<DBColumnSchema>>(DBTableSchemaPtr->EnumColumn());
		}
		else
		{
			InnerEnumPtr = std::auto_ptr<IEnumerator<DBColumnSchema>>(0);
		}
	}

	virtual ICloneable* Clone() const
	{
		return new CDBColumnInfoEnumerator(*this);
	}

protected:
	CDBTableSchema*					DBTableSchemaPtr;
	std::auto_ptr<IEnumerator<DBColumnSchema>>	InnerEnumPtr;
};