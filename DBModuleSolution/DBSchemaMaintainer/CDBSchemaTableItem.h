#pragma once

#include "DBInterface/DBDataAdapter.h"
#include "Module/DBRecord.h"
#include "EnumEntity.h"
#include "Module/Schema/DBColumnSchema.h"
#include <boost/lexical_cast.hpp>

BEGINENUM(ModifyState, 状态)

ENUMITEM(Unknow, 未知, 0)
ENUMITEM(Uptodate, 最新,1)
ENUMITEM(Add, 添加,2)
ENUMITEM(Remove, 移除,3)
ENUMITEM(Modify, 更改,4)

ENDENUM

BEGINENUM(DBSchemaItemCategory, 类型)

//ENUMITEM(Unknow, 未知, 0)
ENUMITEM(Table, 表, 1)
ENUMITEM(Column, 列, 2)

ENDENUM

using namespace NSDBModule;

class CDBSchemaItem : public IDBRecord
{
public:
	virtual const tstring& GetField(unsigned int i) const 
	{
		switch(i)
		{
		case 0:
			return DBName;
			break;
		case 1:
			return Name;
			break;
		case 2:
			return boost::lexical_cast<tstring>(ModifyState);
			break;
		default:
		case 3:
			_ASSERT(false);
			break;
		}
		return tstring();
	}

	virtual int	SetField(unsigned int i, const tstring& val)
	{
		switch(i)
		{
		case 0:
			DBName = val;
			break;
		case 1:
			Name = val;
			break;
		case 2:
			ModifyState = (EnumModifyState)boost::lexical_cast<int>(val.c_str());
			break;
		default:
			_ASSERT(false);
			break;
		}
		return 1;
	}

	virtual index_t	GetFieldCount() const { return 3; };

public:
	tstring						DBName;
	tstring						Name;
	tstring						Table;
	EnumModifyState				ModifyState;
	EnumDBSchemaItemCategory	Category;
	EnumCppDataType				CppType;
	IDBDataType*				DBType;
};

class CDBSchemaTableItem : public IDBRecord
{
public:
	virtual const tstring& GetField(unsigned int i) const 
	{
		switch(i)
		{
		case 0:
			return DBName;
			break;
		case 1:
			return Name;
			break;
		case 2:
			return boost::lexical_cast<tstring>(ModifyState);
			break;
		default:
			_ASSERT(false);
			break;
		}
		return tstring();
	}

	virtual int	SetField(unsigned int i, const tstring& val)
	{
		switch(i)
		{
		case 0:
			DBName = val;
			break;
		case 1:
			Name = val;
			break;
		case 2:
			ModifyState = (EnumModifyState)boost::lexical_cast<int>(val.c_str());
			break;
		default:
			_ASSERT(false);
			break;
		}
		return 1;
	}

	virtual index_t	GetFieldCount() const { return 3; };

public:
	tstring			DBName;
	tstring			Name;
	EnumModifyState	ModifyState;
};

class CDBSchemaColumnItem : public IDBRecord
{
public:
	virtual const tstring& GetField(unsigned int i) const 
	{
		switch(i)
		{
		case 0:
			return DBName;
			break;
		case 1:
			return Name;
			break;
		case 2:
			return boost::lexical_cast<tstring>(ModifyState);
			break;
		case 3:
			return KeyColumn ? TEXT("1") : TEXT ("0");
		default:
			_ASSERT(false);
			break;
		}
		_ASSERT(false);
		return tstring();
	}

	virtual int	SetField(unsigned int i, const tstring& val)
	{
		switch(i)
		{
		case 0:
			DBName = val;
			break;
		case 1:
			Name = val;
			break;
		case 2:
			ModifyState = (EnumModifyState)boost::lexical_cast<int>(val.c_str());
			break;
		case 3:
			KeyColumn = ((val == TEXT("0")) ? false : true);
			break;
		default:
			_ASSERT(false);
		}
		return 1;
	}

	virtual index_t	GetFieldCount() const { return 4; };

public:
	tstring			DBName;
	tstring			Name;
	EnumModifyState	ModifyState;
	bool			KeyColumn;
};
