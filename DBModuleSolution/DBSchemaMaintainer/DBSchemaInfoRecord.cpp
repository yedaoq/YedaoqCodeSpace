#include "stdafx.h"
#include "DBSchemaInfoRecord.h"
#include "Module\Schema\DBSchemaValidater.h"
#include "Module\DBTable.h"
#include "Module\Schema\DBColumnSchema.h"
#include "DBInterface\DBDataType.h"
#include "Module\EnumCppDataType.h"
#include <boost\lexical_cast.hpp>

const tstring& CDBTableInfoRecord::GetField(unsigned int idx) const
{
	static tstring str;

	if(!TblPtr)
	{
		_ASSERT(false);
		str.clear();
		return str;
	}

	switch (idx)
	{
	case Name:
		return TblPtr->GetName();
		break;
	case DBName:
		return TblPtr->GetDBName();
		break;
	case Buildin:
		str = TblPtr->GetSchema().IsBuildin() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case DBExist:
		str = TblPtr->GetSchema().IsDBExist() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case State:
		str = IsStateNormal() ? TEXT("") : TEXT("!");
		break;
	default:
		_ASSERT(false);
		str.clear();
		return str;
		break;
	}

	return str;
}

int	CDBTableInfoRecord::SetField(unsigned int idx, const tstring& val)
{
	if(!TblPtr)
	{
		_ASSERT(false);
		return -1;
	}

	switch (idx)
	{
	case Name:
		TblPtr->GetSchema().Name = val;
		break;
	case DBName:
		TblPtr->GetSchema().DBName = val;
		break;
	case Buildin:
		TblPtr->GetSchema().SetFlag(CDBTableSchema::BuildIn, val == TEXT("1"));
		break;
	case DBExist:
		TblPtr->GetSchema().SetFlag(CDBTableSchema::DBExist, val == TEXT("1"));
		break;
	case State:
		break;
	default:
		_ASSERT(false);
		break;
	}

	return 1;
}

bool CDBTableInfoRecord::IsStateNormal() const
{
	return CDBSchemaValidater::ValidateTable(TblPtr->GetSchema());
}

const tstring& CDBColumnInfoRecord::GetField(unsigned int idx) const
{
	static tstring str;

	if(!ColPtr)
	{
		_ASSERT(false);
		str.clear();
		return str;
	}

	switch (idx)
	{
	case Name:
		return ColPtr->Name;
		break;
	case DBName:
		return ColPtr->DBName;
		break;
	case Buildin:
		str = ColPtr->IsBuildin() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case DBExist:
		str = ColPtr->IsDBExist() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case KeyCol:
		str = ColPtr->IsKeyColumn() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case DBPK:
		str = ColPtr->IsDBPrimaryKey() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case DBUnnull:
		str = ColPtr->IsDBUnnull() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case Type:
		str = EnumEntityOfCppDataType()[ColPtr->Type].ValueStr; // str.clear(); //
		return str;
		break;
	case DBType:
		str = ColPtr->DBType->ToString();
		return str;
		break;
	case State:
		str = IsStateNormal() ? TEXT("") : TEXT("!");
		break;
	case RelyTbl:
		str = boost::lexical_cast<tstring>(ColPtr->RelyTblID);
		break;
	case RelyCol:
		str = boost::lexical_cast<tstring>(ColPtr->RelyColID);
		break;
	case VisiCol:
		str = boost::lexical_cast<tstring>(ColPtr->VisiColID);
		break;
	case Index:
		str = boost::lexical_cast<tstring>(ColPtr->Index);
		break; 
	default:
		_ASSERT(false);
		str.clear();
		return str;
		break;
	}

	return str;
}

int	CDBColumnInfoRecord::SetField(unsigned int idx, const tstring& val)
{
	if(!ColPtr)
	{
		_ASSERT(false);
		return -1;
	}

	switch (idx)
	{
	case Name:
		ColPtr->Name = val;
		break;
	case DBName:
		ColPtr->DBName = val;
		break;
	case Buildin:
		ColPtr->SetFlag(EnumDBColumnSchemaFlag::BuildIn, val == TEXT("1"));
		break;
	case DBExist:
		ColPtr->SetFlag(EnumDBColumnSchemaFlag::DBExist, val == TEXT("1"));
		break;
	case KeyCol:
		ColPtr->SetFlag(EnumDBColumnSchemaFlag::KeyColumn, val == TEXT("1"));
		break;
	case DBPK:
		ColPtr->SetFlag(EnumDBColumnSchemaFlag::DBPrimaryKey, val == TEXT("1"));
		break;
	case DBUnnull:
		ColPtr->SetFlag(EnumDBColumnSchemaFlag::DBUnnull, val == TEXT("1"));
		break;
	case Type:
		ColPtr->Type = EnumEntityOfCppDataType()[val].Value<EnumCppDataType>();
		break;
	case DBType:
		//str = ColPtr->DBType->ToString();
		break;
	case State:
		//str = IsStateNormal() ? TEXT("1") : TEXT("0");
		break;
	case RelyTbl:
		ColPtr->RelyTblID = boost::lexical_cast<index_t>(val);
		break;
	case RelyCol:
		ColPtr->RelyColID = boost::lexical_cast<index_t>(val);
		break;
	case VisiCol:
		ColPtr->VisiColID = boost::lexical_cast<index_t>(val);
		break;
	case Index:
		_ASSERT(false);
		break;
	default:
		_ASSERT(false);
		break;
	}

	return 1;
}

bool CDBColumnInfoRecord::IsStateNormal() const
{
	return CDBSchemaValidater::ValidateColumn(*ColPtr);
}
