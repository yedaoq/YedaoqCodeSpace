#pragma once

#include "mytype.h"
#include "DBTableViewer4GridCtrl.h"

interface IDwarfViewInfo;

using namespace NSDBModule;

enum EnumRecordRange
{
	None,
	SelectedOnes,
	All,
};

struct DwarfViewOperationContext
{
	__in	int								MainViewID;
	__in	CDBRecordAuto*					pFocusedMainRecord;
	__in	IEnumerator<IDBRecord>*			pSelectedRecords;
	__in	IEnumerator<DBTableViewColumn>*	pSelectedColumns;

	__in	int								SideViewID;
	__in	CDBRecordAuto*					pFocusedSideRecord;
	__in	IEnumerator<IDBRecord>*			pSelectedSideRecords;
	__in	IEnumerator<DBTableViewColumn>* pSelectedSideColumns;

	__out	EnumRecordRange					RecordsToRefresh;
	__out	bool							SideVideRefresh;
};

typedef void (AFX_MSG_CALL IDwarfViewInfo::*DelegateOperation)(DwarfViewOperationContext* pCtx);

struct DwarfViewOperationItem
{
	index_t				ID;
	tstring				Name;
	DelegateOperation	Operation;

	DwarfViewOperationItem()
		: ID(0), Operation(0)
	{}

	DwarfViewOperationItem(index_t id, tstring name, DelegateOperation op)
		: ID(id), Name(name), Operation(op)
	{}
};

interface IDwarfViewInfo
{
	virtual int										GetViewID() = 0;
	virtual tstring									ToString() = 0;

	virtual int										Initialize() = 0;
	virtual bool									Initialized() const = 0;

	virtual CDBTableViewColumnCollection&			GetViewColumnCollection() = 0;

	virtual IEnumerator<IDwarfViewInfo*>*			EnumReleatedView() = 0;

	virtual IEnumerator<IDBRecord>*					EnumRecordAsRelatedView(IDwarfViewInfo* pView, DwarfViewOperationContext* pCtx) = 0;
	virtual IEnumerator<IDBRecord>*					EnumRecord() = 0;

	virtual IEnumerator<DwarfViewOperationItem>*	EnumOperation() = 0;
	virtual void									ExecuteOperation(index_t id, DwarfViewOperationContext* pCtx) = 0;
};