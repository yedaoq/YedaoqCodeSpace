#pragma once
#include <mytype.h>
#include <View\DBColumnViewInfo.h>
#include <Module\DBModule.h>
#include <Module\DBTable.h>
#include "DwarfViewInfoBase.h"
#include "DBTableViewer4GridCtrl.h"
#include "DwarfViewInfo.h"


using namespace NSDBModule;

class CDwarfViewInfoDBTblBase : public CDwarfViewInfoBase
{
public:
	CDwarfViewInfoDBTblBase(CDBModule* module, index_t id)
		: CDwarfViewInfoBase(id), DBModule(module)
	{}

	~CDwarfViewInfoDBTblBase(void);

	virtual tstring	ToString() { return DBModule->Tables()[ViewID]->GetName(); }

	virtual IEnumerator<IDBRecord>*	EnumRecordAsRelatedView(DwarfViewOperationContext* pCtx);
	virtual IEnumerator<IDBRecord>*	EnumRecord();

	virtual int Initialize();
	virtual int InitializeReleatedViews();
	virtual int InitializeViewColumns();
	virtual int InitializeColumnViewInfo();
	virtual int InitializeOperations();

	virtual afx_msg void OnRecordModify(DwarfViewOperationContext* pCtx); 
	virtual afx_msg void OnRecordDelete(DwarfViewOperationContext* pCtx); 
	virtual afx_msg void OnRecordInsert(DwarfViewOperationContext* pCtx); 

	virtual CDBColumnViewInfo GenerateColumnViewFromSchema(const DBColumnSchema& col);

	// 判断两表是否关联
	static bool IsDBTableRelated(int tblCur, int tblTar, int* colCur, int* colTar);

protected:	
	CDBModule* DBModule;
};
