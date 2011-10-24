#pragma once
#include <mytype.h>
#include <View\DBColumnViewInfo.h>
#include <Module\DBModule.h>
#include "DwarfViewInfoBase.h"
#include "DBTableViewer4GridCtrl.h"
#include "DwarfViewInfo.h"

using namespace NSDBModule;

class CDwarfViewInfoDBTblBase : public CDwarfViewInfoBase
{
public:
	CDwarfViewInfoDBTblBase(CDBModule* module, index_t id)
		: DBModule(module)
	{
		ViewID = id;
	}

	~CDwarfViewInfoDBTblBase(void);

	virtual int	GetViewID() { return ViewID; }

	virtual int Initialize();
	virtual int InitializeReleatedViews();
	virtual int InitializeViewColumns();
	virtual int InitializeColumnViewInfo();
	virtual int InitializeOperations();

	virtual afx_msg void OnRecordModify(DwarfViewOperationContext* pCtx); 
	virtual afx_msg void OnRecordDelete(DwarfViewOperationContext* pCtx); 
	virtual afx_msg void OnRecordInsert(DwarfViewOperationContext* pCtx); 

	virtual CDBColumnViewInfo GenerateColumnViewFromSchema(const DBColumnSchema& col);

protected:	
	CDBModule* DBModule;
};
