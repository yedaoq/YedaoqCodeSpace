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
	__in	IEnumerator<IDBRecord>*			pSelectedRecords;
	__in	IEnumerator<DBTableViewColumn>*	pSelectedColumns;
	__out	EnumRecordRange*				pRecordsToRefresh;
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
	virtual CDBTableViewColumnCollection&			GetViewColumnCollection() = 0;

	virtual IEnumerator<IDwarfViewInfo*>*			EnumReleatedView() = 0;

	virtual IEnumerator<IDBRecord>*					EnumRecord4RelatedView(IDBRecord& item) = 0;
	virtual IEnumerator<IDBRecord>*					EnumRecord() = 0;

	virtual IEnumerator<DwarfViewOperationItem>*	EnumOperation() = 0;
	virtual void									ExecuteOperation(index_t id, DwarfViewOperationContext* pCtx);
};