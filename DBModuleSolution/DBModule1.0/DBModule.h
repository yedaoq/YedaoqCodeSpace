#pragma once
#include "DBDataAdapter.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface IDBProvider;
	interface IDBDataAdapter;

class CDBModule
{
public:
	CDBModule(void);
	virtual int Initialize(IDBProvider& provider);

	virtual IDBDataAdapter* DBAdapter() { return DBAdapter_; }
	virtual int	SetDataAdapter(IDBDataAdapter* adapter, bool bResetSchema, bool bResetData);

protected:
	IDBDataAdapter* DBAdapter_;
};

}