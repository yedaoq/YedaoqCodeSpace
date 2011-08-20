#pragma once
#include "vector"
#include "Enumerator.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface	IDBFactory;
	interface	IDBDataAdapter;
	class		CDBTable;
	class		CBuildInSchemaValidater;

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

	typedef std::vector<CDBTable>								DBTableCollection;
	typedef CIteratorEnumerator<DBTableCollection::iterator>	DBTableEnumerator;

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
	//virtual bool			Initialized() const {return Initialized_; }				// whether the module has initialized
	virtual bool			Accessable() const {return BindingToAnyDataBase() && Valid == SchemaValidity(); /*&& Initialized()*/ }

	DBTableEnumerator		EnumTable() const;

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
	CBuildInSchemaValidater Validater_;
};

}