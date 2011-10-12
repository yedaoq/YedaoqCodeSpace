
// redef micro BEGIN_DBSCHEMA
#ifdef BEGIN_DBSCHEMA
#undef BEGIN_DBSCHEMA
#endif

#define BEGIN_DBSCHEMA(name, version) int CDB##name:: InitializeBuildinSchema() { \
	index_t anonyousTableIndex = 0; \
	DBColumnSchema anonyousColumnSchema; //\
//	CDBTableSchema anonyousTableSchema; 
	

// redef micro END_DBSCHEMA
#ifdef END_DBSCHEMA
#undef END_DBSCHEMA
#endif

#define END_DBSCHEMA }

// redef micro BEGIN_DBTABLE
#ifdef BEGIN_DBTABLE
#undef BEGIN_DBTABLE
#endif

#define BEGIN_DBTABLE(name) TBL_##name = anonyousTableIndex++; \
	anonyousTableSchema.Clear(true); 

// redef micro END_DBTABLE
#ifdef END_DBTABLE
#undef END_DBTABLE
#endif

#define END_DBTABLE Tables().Append(anonyousTableSchema, this, true);

// redef micro DBCOLUMN
#ifdef DBCOLUMN
#undef DBCOLUMN
#endif

#define DBCOLUMN(tbl ,name, type, flag, uniquemask) {\
	DBColumnSchema anonyousColumnSchema = {0, TEXT(#name), (EnumCppDataType)type, 0, TEXT(""), 0, uniquemask, flag}; \
	anonyousColumnSchema.ResetExternInfo();	\
	anonyousTableSchema.AppendColumn(anonyousColumnSchema);}