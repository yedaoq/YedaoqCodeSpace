#pragma once
#include "vector"

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
		SchemaConflict			// the buildin schema is conflict with the schema of database attached
	};

	typedef std::vector<CDBTable> DBTableCollection;

public:
	CDBModule(void)
		: DBAdapter_(0), DBFactory_(0), WithBuildinSchema_(false), Tables_(0)
	{}

	// buildin schema initialize
	virtual int				InitializeBuildinSchema() { WithBuildinSchema_ = false; }

	// database binding
	virtual	int				AttachToDatabase(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory);
	virtual int				DetachFromDataBase();

	// initialize for access
	virtual int				Initialize();	

	// module info
	virtual IDBDataAdapter* DBAdapter() { return DBAdapter_; }			// get the database data adapter
	virtual IDBFactory*		DBFactory() { return DBFactory_; }			// get the database component factory
	virtual bool			WithBuildinSchema() {return WithBuildinSchema_; }	// whether the module has a buildin schema
	
	// module state
	virtual bool			BindingToAnyDataBase() { return 0 != DBAdapter_; }	// whether the module has binding to any database
	virtual EnumSchemaValidity SchemaValidity() {return SchemaValidity_; }		// get the schema validation state
	virtual bool			Initialized() {return Initialized_; }				// whether the module has initialized
	
	virtual bool			Accessable() {return BindingToAnyDataBase() && Valid == SchemaValidity() && Initialized() }

	// schema operation
	virtual int				ValidateSchema();
	virtual int				ClearSchema();
	virtual int				RefreshSchema();
	
	// data operation
	virtual int				ClearData();
	virtual int				RefreshData();

protected:
	IDBDataAdapter*			DBAdapter_;
	IDBFactory*				DBFactory_;
	bool					WithBuildinSchema_;
	EnumSchemaValidity		SchemaValidity_;
	bool					Initialized_;

	DBTableCollection		Tables_;
};

}