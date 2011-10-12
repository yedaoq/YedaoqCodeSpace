#include "..\Module\DBTable.h"
#include "..\Module\DBModule.h"
#include "..\Module\DBRecordComparison.h"
#include "..\DBInterface\DBCommandBuilder.h"
#include "..\DBInterface\DBDataAdapter.h"
#include <crtdbg.h>

using namespace NSDBModule;

#define DBPREPARE												\
	{															\
		if(!DBModule_ || !DBModule_->DBAdapter())				\
		{														\
			_ASSERT(DBModule_ && DBModule_->DBAdapter());		\
			throw std::exception();								\
		}														\
		if(!FlagLoaded_)										\
		{														\
			LoadData();											\
		}														\
	}

CDBTable::CDBTable(CDBModule* module)
	: DBModule_(module), FlagLoaded_(false), LessThan_(Comparison_), Records_(LessThan_)	  
{}

CDBTable::CDBTable(CDBModule* module, const CDBTableSchema& schema)
	: DBModule_(module), Schema_(schema), FlagLoaded_(false), LessThan_(Comparison_), Records_(LessThan_)	  
{}



CDBTable::CDBTable(const CDBTable& other)
	: DBModule_(other.DBModule_), FlagLoaded_(false), LessThan_(Comparison_), Records_(LessThan_)	
{}

int CDBTable::Initialize()
{
	return 0;
}

IEnumerator<DBColumnSchema>* CDBTable::EnumColumn() const
{
	return Schema_.EnumColumn();
}

DBRecordEnumerator CDBTable::EnumRecord() /*const*/
{
	if(!FlagLoaded_)
	{
		LoadData();
	}

	return make_iterator_enumerator(Records_.begin(), Records_.end());
}

int CDBTable::ClearData()
{
	Records_.clear();
	FlagLoaded_ = false;

	return 1;
}

int	CDBTable::LoadData()
{
	if(FlagLoaded_ && !FlagDirty_)
	{
		return 1;
	}

	Records_.clear();

	if(!DBModule_ || !DBModule_->DBAdapter())
	{
		_ASSERT(DBModule_ && DBModule_->DBAdapter());
		throw std::exception();
	}	
	
	IDBCommand*			cmd;
	int					iRet = 0;
	iRet = CommandBuilder_->GetCmdSelect(&cmd);
	if(iRet <= 0)
	{
		_ASSERT(iRet > 0);
		throw std::exception();
	}
	
	//return DBModule_->DBAdapter()->Fill(Records_);

	return 0;
}

int	CDBTable::Find(IDBRecord& rec) /*const*/
{
	if(!FlagLoaded_)
	{
		LoadData();
	}

	DBRecordIterator iter = Records_.find(rec);
	if(iter != Records_.end())
	{
		rec = *iter;
		return 1;
	}

	return 0;
}

int	CDBTable::Find(IDBRecord& rec, const CDBRecordComparison& cmp)
{
	if(!FlagLoaded_)
	{
		LoadData();
	}

	for (DBRecordIterator iter = Records_.begin(); iter != Records_.end(); ++iter)
	{
		if(0 == cmp(rec, *iter))
		{
			rec = *iter;
			return 1;
		}
	}
	return 0;
}

DBRecordEnumerator CDBTable::FindAll(const IDBRecord& rec, const CDBRecordComparison& cmp) /*const*/
{
	if(!FlagLoaded_)
	{
		LoadData();
	}
	throw std::exception();
	//return make_iterator_enumerator();
}

int	CDBTable::Update(const IDBRecord& cur, const IDBRecord& ori)
{
	DBPREPARE;

	DBRecordIterator iOri = Records_.find(ori);
	if(iOri == Records_.end())
	{
		// data non exist
		return 0;
	}

	if(0 != Comparison_(cur, ori))
	{
		if(Records_.find(cur) != Records_.end())
		{
			// data repeat
			return 0;
		}
	}

	// update to database
	IDBCommand* cmd;
	CommandBuilder_->GetCmdUpdate(cur, ori, Comparison_, &cmd);

	if(DBModule_->DBAdapter()->Execute(*cmd))
	{
		// database fail
		return 0;
	}

	// update memory data
	Records_.erase(iOri);
	Records_.insert(cur);

	return 1;
}

int	CDBTable::Insert(const IDBRecord& rec)
{
	DBPREPARE;

	DBRecordIterator iOri = Records_.find(rec);
	if(iOri != Records_.end())
	{
		// data exist
		return 0;
	}

	// update to database
	IDBCommand* cmd;
	CommandBuilder_->GetCmdInsert(rec, &cmd);

	if(DBModule_->DBAdapter()->Execute(*cmd))
	{
		// database fail
		return 0;
	}

	// update memory data
	Records_.insert(rec);

	return 1;
}

int	CDBTable::Delete(const IDBRecord& rec)
{
	DBPREPARE;

	DBRecordIterator iOri = Records_.find(rec);
	if(iOri == Records_.end())
	{
		// data non exist
		return 0;
	}

	// update to database
	IDBCommand* cmd;
	CommandBuilder_->GetCmdDelete(rec, Comparison_, &cmd);

	if(DBModule_->DBAdapter()->Execute(*cmd))
	{
		// database fail
		return 0;
	}

	// update memory data
	Records_.erase(iOri);

	return 1;
}