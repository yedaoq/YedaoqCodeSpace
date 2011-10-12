#include "stdafx.h"
#include "DBSchemaInfoRecord.h"
#include "Module\Schema\DBSchemaValidater.h"
#include "Module\DBTable.h"
#include "Module\Schema\DBColumnSchema.h"
#include "DBInterface\DBDataType.h"
#include "Module\CPPDataType.h"

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
	case 1:
		return TblPtr->GetName();
		break;
	case 2:
		return TblPtr->GetDBName();
		break;
	case 3:
		str = TblPtr->GetSchema().IsBuildin() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case 4:
		str = TblPtr->GetSchema().IsDBExist() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case 0:
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
	case 1:
		TblPtr->GetSchema().Name = val;
		break;
	case 2:
		TblPtr->GetSchema().DBName = val;
		break;
	case 3:
		TblPtr->GetSchema().SetFlag(CDBTableSchema::BuildIn, val == TEXT("1"));
		break;
	case 4:
		TblPtr->GetSchema().SetFlag(CDBTableSchema::DBExist, val == TEXT("1"));
		break;
	case 0:
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
	case 1:
		return ColPtr->Name;
		break;
	case 2:
		return ColPtr->DBName;
		break;
	case 3:
		str = ColPtr->IsBuildin() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case 4:
		str = ColPtr->IsDBExist() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case 7:
		str = ColPtr->IsKeyColumn() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case 8:
		str = ColPtr->IsDBPrimaryKey() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case 9:
		str = ColPtr->IsDBNullable() ? TEXT("1") : TEXT("0");
		return str;
		break;
	case 5:
		str = EnumEntityOfCppDataType()[ColPtr->Type].ValueStr;
		return str;
		break;
	case 6:
		str = ColPtr->DBType->ToString();
		return str;
		break;
	case 0:
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

int	CDBColumnInfoRecord::SetField(unsigned int idx, const tstring& val)
{
	if(!ColPtr)
	{
		_ASSERT(false);
		return -1;
	}

	switch (idx)
	{
	case 1:
		ColPtr->Name = val;
		break;
	case 2:
		ColPtr->DBName = val;
		break;
	case 3:
		ColPtr->SetFlag(DBColumnSchema::BuildIn, TEXT("1"));
		break;
	case 4:
		ColPtr->SetFlag(DBColumnSchema::DBExist, TEXT("1"));
		break;
	case 7:
		ColPtr->SetFlag(DBColumnSchema::KeyColumn, TEXT("1"));
		break;
	case 8:
		//ColPtr->SetFlag(DBColumnSchema::DBPrimaryKey, TEXT("1"));
		break;
	case 9:
		//ColPtr->SetFlag(DBColumnSchema::DBNullable, TEXT("1"));
		break;
	case 5:
		break;
	case 6:
		//str = ColPtr->DBType->ToString();
		break;
	case 0:
		//str = IsStateNormal() ? TEXT("1") : TEXT("0");
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
