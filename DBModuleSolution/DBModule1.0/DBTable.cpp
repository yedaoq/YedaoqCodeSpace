#include "DBTable.h"
#include "DBModule.h"
#include "DBCommandBuilder.h"
#include "DBRecordComparison.h"
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

CDBTable::CDBTable()
	: FlagLoaded_(false), LessThan_(Comparison_), Records_(LessThan_)	  
{}

int CDBTable::Initialize()
{

}

ColumnSchemaEnumPtr CDBTable::EnumColumn()
{
	return Schema_.GetColumnSchemaEnumerator();
}

DBRecordEnumPtr	CDBTable::EnumRecord()
{
	if(!FlagLoaded_)
	{
		LoadData();
	}

	return DBRecordEnumPtr(new_iterator_enumerator(Records_.begin(), Records_.end()));
}

int	CDBTable::LoadData()
{
	if(FlagLoaded_)
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
	
	return DBModule_->DBAdapter()->Fill(Records_);

	return 1;
}

int	CDBTable::Find(CDBRecordBase& rec)
{
	if(!FlagLoaded_)
	{
		LoadData();
	}

	RecIterator iter = Records_.find(rec);
	if(iter != Records_.end())
	{
		rec = *iter;
		return 1;
	}

	return 0;
}

int	CDBTable::Find(CDBRecordBase& rec, const CDBRecordComparison& cmp)
{
	if(!FlagLoaded_)
	{
		LoadData();
	}

	for (RecIterator iter = Records_.begin(); iter != Records_.end(); ++iter)
	{
		if(0 == cmp(rec, *iter))
		{
			rec = *iter;
			return 1;
		}
	}
	return 0;
}

RecEnumPtr CDBTable::FindAll(const CDBRecordBase& rec, const CDBRecordComparison& cmp)
{
	if(!FlagLoaded_)
	{
		LoadData();
	}


}

int	CDBTable::Update(const CDBRecordBase& cur, const CDBRecordBase& ori)
{
	DBPREPARE;

	RecIterator iOri = Records_.find(ori);
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
	CommandBuilder_->GetCmdUpdate(cur, ori, &cmd);

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

int	CDBTable::Insert(const CDBRecordBase& rec)
{
	DBPREPARE;

	RecIterator iOri = Records_.find(rec);
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

int	CDBTable::Delete(const CDBRecordBase& rec)
{
	DBPREPARE;

	RecIterator iOri = Records_.find(rec);
	if(iOri == Records_.end())
	{
		// data non exist
		return 0;
	}

	// update to database
	IDBCommand* cmd;
	CommandBuilder_->GetCmdDelete(rec, &cmd);

	if(DBModule_->DBAdapter()->Execute(*cmd))
	{
		// database fail
		return 0;
	}

	// update memory data
	Records_.erase(iOri);

	return 1;
}