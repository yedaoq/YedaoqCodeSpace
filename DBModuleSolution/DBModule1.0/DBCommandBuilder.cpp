#include "DBCommandBuilder.h"
#include "DBTableSchema.h"

using namespace NSDBModule;

int CDBCommandBuilderBase::Initialize(CDBTableSchema const* schema)
{

}

int CDBCommandBuilderBase::GetCmdSelect(const CDBRecordBase&, const CDBRecordComparison&, IDBCommand**)
{

}

int CDBCommandBuilderBase::GetCmdSelect(IDBCommand**)
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