#include "DBCommandBuilder.h"
#include "DBTableSchema.h"
#include <crtdbg.h>
#include "DBCommand.h"
#include <winnt.h>

using namespace NSDBModule;

int CDBCommandBuilderBase::Initialize(CDBTableSchema const* schema)
{
	
}

int CDBCommandBuilderBase::GetCmdSelect(const CDBRecordBase& rec, const CDBRecordComparison& cmp, IDBCommand** cmd)
{
	if(!TableSchema_)
	{
		_ASSERT(false);
		throw std::exception();
	}

	*cmd = new CDBCommandBase(EnumDBCommandType::Sql);
	tstring strSql;
	strSql.append(TEXT("SELECT * FROM "));
	strSql.append(TableSchema_.Name);
	
}

int CDBCommandBuilderBase::GetCmdSelect(IDBCommand** cmd)
{

}

int CDBCommandBuilderBase::GetCmdInsert(const CDBRecordBase&, IDBCommand**)
{

}

int CDBCommandBuilderBase::GetCmdDelete(const CDBRecordBase&, IDBCommand**)
{

}

int CDBCommandBuilderBase::GetCmdUpdate(const CDBRecordBase&, const CDBRecordBase&, IDBCommand**)
{

}

int CDBCommandBuilderBase::GetCmdCreate()
{

}

int CDBCommandBuilderBase::GenerateConditionStr(const CDBRecordBase& rec, const CDBRecordComparison& cmp, tstring& buffer)
{
	if(cmp.KeyFields_.size() > 0)
	{
		buffer.append(TEXT(" WHERE "));
		
		for (int i = 0; i < cmp.KeyFields_.size(); ++i)
		{

		}
		
	}
}

virtual int CDBCommandBuilderBase::GenerateFieldAssignmentStr(const CDBRecordBase& rec, index_t field, tstring& buffer)
{
	if(field >= rec.GetFieldCount())
	{
		ASSERT(field < rec.GetFieldCount());
		return 0;
	}
	
	buffer.append(TableSchema_[field].DBName);
}