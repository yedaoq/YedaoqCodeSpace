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
	strSql += TEXT("SELECT * FROM ");
	strSql += this->TableSchema_.Name;
	strSql += TEXT(" WHERE ");
	strSql += 
	
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

int CDBCommandBuilderBase::GenerateConditionStr(const CDBRecordBase& rec, const CDBRecordComparison& cmp)
{

}