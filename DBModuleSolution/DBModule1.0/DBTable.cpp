#include "DBTable.h"
#include "DBModule.h"
#include "DBCommandBuilder.h"
#include <..\crt\src\utils.h>
#include "DBRecordComparison.h"

using namespace NSDBModule;

CDBTable::CDBTable()
	: FlagLoaded_(false), LessThan_(Comparison_), Records_(LessThan_)	  
{

}

ColumnSchemaEnumPtr CDBTable::EnumColumn()
{
	return Schema_.GetColumnSchemaEnumerator();
}

DBRecordEnumPtr	CDBTable::EnumRecord()
{
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
		ASSERT(DBModule_ && DBModule_->DBAdapter());
		throw std::exception();
	}	
	
	CDBRecordComparison cmp(0);	// constructor a comaprison there with no column compare
	CDBRecordAuto		rec;
	IDBCommand*			cmd;
	int					iRet = 0;
	iRet = CommandBuilder_->GetCmdSelect(rec, cmp, &cmd);
	if(iRet <= 0)
	{
		ASSERT(iRet > 0);
		throw std::exception();
	}
	
	return DBModule_->DBAdapter()->Fill(Records_);

	return 1;
}

int	CDBTable::Find(CDBRecordBase& rec)
{
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

int	CDBTable::Update(const CDBRecordBase& cur, const CDBRecordBase& ori)
{
	
}

int	CDBTable::Insert(const CDBRecordBase& rec)
{

}

int	CDBTable::Delete(const CDBRecordBase& rec)
{

}