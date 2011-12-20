#pragma once

#include "DwarfViewInfoDBTable.h"
#include "LocalPlayerObjEnum.h"

template<>
class CDwarfViewInfoDBTable<TBL_DmlLog> : public CDwarfViewInfoDBTblBase
{
public:
	CDwarfViewInfoDBTable();

	virtual int InitializeOperations();

	afx_msg void OnNewVersion(DwarfViewOperationContext* pCtx); 

	void DMLRing(NSDBModule::DMLEvent* e);

	tstring GetLogRecordStr( const IDBRecord* rec );


public:
	tstring DBVersion;
	int		MonitorID;
};
