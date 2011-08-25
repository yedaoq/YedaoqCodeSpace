#pragma once
#include "vector"
#include "Enumerator.h"
#include "DBSchemaValidater.h"
#include "DBSchemaLoader.h"
#include <atlcomcli.h>
#include <memory>
#include "DBTableCollection.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface	IDBFactory;
	interface	IDBDataAdapter;
	class		CDBTable;

class CDBModule
{
public:
	enum EnumSchemaValidity
	{
		Unknow,					// have not validate the schema yet
		Valid,					// the schema is valid
		InvalidBuildinSchema,	// the buildin schema is invalidate
		SchemaConflict,			// the buildin schema is conflict with the schema of database attached
	};

public:
	CDBModule(void)
		: DBAdapter_(0), DBFactory_(0), WithBuildinSchema_(false), Tables_(0), Validater_(*this)
	{}

	// initialize for access	
	virtual int				InitializeBuildinSchema() { WithBuildinSchema_ = false; } //// buildin schema initialize
	virtual int				Initialize();	

	// database binding
	virtual	int				AttachToDatabase(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory);
	virtual int				DetachFromDataBase();

	// module info
	virtual IDBDataAdapter* DBAdapter() const { return DBAdapter_; }			// get the database data adapter
	virtual IDBFactory*		DBFactory() const { return DBFactory_; }			// get the database component factory
	virtual bool			WithBuildinSchema() const {return WithBuildinSchema_; }	// whether the module has a buildin schema
	
	// module state
	virtual bool			BindingToAnyDataBase() const { return 0 != DBAdapter_; }	// whether the module has binding to any database
	virtual EnumSchemaValidity SchemaValidity() const {return SchemaValidity_; }		// get the schema validation state
	virtual bool			Accessable() const {return BindingToAnyDataBase() && Valid == SchemaValidity(); /*&& Initialized()*/ }

	// schema operation
	virtual int				Clear();
	virtual int				RefreshSchema();

	// data operation
	virtual int				ClearData();
	virtual int				RefreshData();

	CDBTableCollection&		Tables() { return Tables_; }
	const CDBTableCollection& Tables() const { return Tables_; }
	
protected:
	IDBDataAdapter*			DBAdapter_;
	IDBFactory*				DBFactory_;
	bool					WithBuildinSchema_;
				
	EnumSchemaValidity		SchemaValidity_;

	CDBTableCollection		Tables_;
	CDBSchemaValidater		Validater_;
};

}