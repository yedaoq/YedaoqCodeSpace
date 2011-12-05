#pragma once
#include "vector"
#include "Enumerator.h"
#include ".\Schema\DBSchemaLoader.h"
#include ".\DMLNotifier\DMLNotifier.h"
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
	interface	IDBNameMapping;
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
		: DBAdapter_(0), DBFactory_(0), WithBuildinSchema_(false)
	{}

	// initialize for access	
	virtual int				InitializeBuildinSchema() { WithBuildinSchema_ = false; return 1; } //// buildin schema initialize
	virtual int				Initialize();	

	// database binding
	virtual	int				AttachToDatabase(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory, IDBNameMapping* dbNameMapping);
	virtual int				DetachFromDataBase();

	// module info
	virtual IDBDataAdapter* DBAdapter() const { return DBAdapter_; }			// get the database data adapter
	virtual IDBFactory*		DBFactory() const { return DBFactory_; }			// get the database component factory
	virtual IDBNameMapping*	DBNameMapping() const { return DBNameMapping_; }	// get the name mapper
	virtual bool			WithBuildinSchema() const {return WithBuildinSchema_; }	// whether the module has a buildin schema
	
	// module state
	virtual bool			BindingToAnyDataBase() const { return 0 != DBAdapter_; }	// whether the module has binding to any database
	virtual bool			SchemaValidity() const {return SchemaValidity_; }		// get the schema validation state
	virtual bool			Accessable() const {return BindingToAnyDataBase() && Valid == SchemaValidity(); /*&& Initialized()*/ }

	// schema operation
	virtual int				Clear(bool bBuildIn = false);
	virtual int				RefreshSchema();

	// data operation
	virtual int				ClearData();
	virtual int				RefreshData();

	CDBTableCollection&		Tables() { return Tables_; }
	const CDBTableCollection& Tables() const { return Tables_; }

	CDMLNotifier&			DMLNotifier() { return DMLNotifier_; }
	
protected:
	IDBDataAdapter*			DBAdapter_;
	IDBFactory*				DBFactory_;
	IDBNameMapping*			DBNameMapping_;

	bool					WithBuildinSchema_;
	bool					SchemaValidity_;

	CDBTableCollection		Tables_;
	CDMLNotifier			DMLNotifier_;
};

}