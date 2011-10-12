
// redef micro BEGIN_DBSCHEMA
#ifdef BEGIN_DBSCHEMA
#undef BEGIN_DBSCHEMA
#endif

#define BEGIN_DBSCHEMA(name, version) class CDB##name : public CDBModule { \
public : \
	virtual int	InitializeBuildinSchema();

// redef micro END_DBSCHEMA
#ifdef END_DBSCHEMA
#undef END_DBSCHEMA
#endif

#define END_DBSCHEMA };

// redef micro BEGIN_DBTABLE
#ifdef BEGIN_DBTABLE
#undef BEGIN_DBTABLE
#endif

#define BEGIN_DBTABLE(name) index_t TBL_##name; \
	enum Enum##name { 

// redef micro END_DBTABLE
#ifdef END_DBTABLE
#undef END_DBTABLE
#endif

#define END_DBTABLE };

// redef micro DBCOLUMN
#ifdef DBCOLUMN
#undef DBCOLUMN
#endif

#define DBCOLUMN(tbl, name, type, flag, uniquemask) COL_##tbl##_##name,