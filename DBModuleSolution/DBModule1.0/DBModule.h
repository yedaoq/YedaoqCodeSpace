#pragma once
#include "DBDataAdapter.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface IDBFactory;
	interface IDBDataAdapter;

class CDBModule
{
public:
	CDBModule(void);

	virtual int Initialize();

	virtual int AttachToDatabase(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory);

	virtual int ClearSchema();
	virtual int RefreshSchema();

	virtual int ClearData();
	virtual int RefreshData();

	virtual IDBDataAdapter* DBAdapter() { return DBAdapter_; }

protected:
	IDBDataAdapter* DBAdapter_;
	bool			
};

}