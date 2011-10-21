#include "..\Module\DBTable.h"
#include "..\Module\DBModule.h"
#include "..\Module\DBRecordComparison.h"
#include "..\DBInterface\DBCommandBuilder.h"
#include "..\DBInterface\DBDataAdapter.h"
#include "..\DBInterface\DBFactory.h"
#include "Schema\DBTableSchema.h"
#include <vector>
#include <crtdbg.h>
#include "DBRecord.h"
#include "DBRecordFunction.h"

using namespace NSDBModule;

#define DBPREPARE														\
	{																	\
		if(!DBModule_ || !DBModule_->DBAdapter() || !CommandBuilder_)	\
		{																\
			_ASSERT(DBModule_ && DBModule_->DBAdapter());				\
			throw std::exception();										\
		}																\
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

int CDBTable::InitializeStore()
{
	// Init record comparison
	CDBTableSchema::ColumnCollection& columns = GetSchema().Columns;
	bool keyColExist = false;
	std::vector<index_t> keyCols(0);
	for (int i = 0; i < columns.size(); ++i)
	{
		if(columns[i].IsKeyColumn())
		{
			if(!keyColExist)
			{
				keyColExist = true;
				keyCols.clear();
			}
			keyCols.push_back(columns[i].Index);
		}
		else if(!keyColExist)
		{
			keyCols.push_back(columns[i].Index);
		}
	}

	Comparison_ = CDBRecordComparison(keyCols);
	
	return 1;
}

int CDBTable::initializeAccess()
{
	CommandBuilder_ = std::auto_ptr<IDBCommandBuilder>(DBModule_->DBFactory()->CreateDBCommandBuilder());
	CommandBuilder_->Initialize(&GetSchema());

	return 1;
}

IEnumerator<DBColumnSchema>* CDBTable::EnumColumn() const
{
	return Schema_.EnumColumn();
}

IEnumerator<IDBRecord>* CDBTable::EnumRecord() /*const*/
{
	if(!FlagLoaded_)
	{
		LoadData();
	}

	return new_iterator_enumerator_ex<IDBRecord>(Records_.begin(), Records_.end());
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

	if(Comparison_.KeyFields().size() <= 0)
	{
		InitializeStore();
	}

	if(!CommandBuilder_.get())
	{
		initializeAccess();
	}

	Records_.clear();

	if(!DBModule_ || !DBModule_->DBAdapter() || !CommandBuilder_.get())	
	{																
		_ASSERT(DBModule_ && DBModule_->DBAdapter());				
		throw std::exception();										
	}																
	
	IDBCommand*	cmd;
	int	iRet = CommandBuilder_->GetCmdSelect(&cmd);

	if(iRet <= 0)
	{
		_ASSERT(iRet > 0);
		throw std::exception();
	}

	IDBDataAdapter::DBRecordEnumPtr pRecs = DBModule_->DBAdapter()->Select(*cmd);
	
	CDBRecordBase rec(GetSchema().Columns.size());
	if(pRecs)
	{
		while(pRecs->MoveNext())
		{
			for (int i = 0; i < GetSchema().Columns.size(); ++i)
			{
				rec.SetField(i, pRecs->Current().GetField(GetSchema()[i].DBIndex));
			}

			Records_.insert(rec);
		}
	}

	return 1;
}

int	CDBTable::Find(IDBRecord& rec) /*const*/
{
	LoadData();

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
	LoadData();

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

IEnumerator<IDBRecord>* CDBTable::FindAll(const IDBRecord& rec, const CDBRecordComparison& cmp) /*const*/
{
	LoadData();

	return new_filter_enumerator(
		make_iterator_enumerator_ex<IDBRecord>(Records_.begin(), Records_.end()),
		CDBRecordFilter(rec, cmp));
}

int	CDBTable::Update(const IDBRecord& cur, const IDBRecord& ori)
{
	LoadData();

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
	LoadData();

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
	LoadData();

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