#pragma once

#include "mytype.h"

interface IDwarfViewInfo;

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

typedef afx_msg void (IDwarfViewInfo::*DelegateOperation)(DwarfViewOperationContext* pCtx);

struct DwarfViewOperationItem
{
	index_t				ID;
	tstring				Name;

	DwarfViewOperationItem()
		: ID(0)
	{}

	DwarfViewOperationItem()
};

interface IDwarfViewInfo
{
	virtual CDBTableViewColumnCollection&			GetViewColumnCollection() = 0;

	virtual IEnumerator<IDwarfViewInfo>*			EnumReleatedView() = 0;

	virtual IEnumerator<IDBRecord>*					EnumRecord4RelatedView(IDBRecord& item) = 0;
	virtual IEnumerator<IDBRecord>*					EnumRecord() = 0;

	virtual IEnumerator<DwarfViewOperationItem>*	EnumOperation() = 0;
	virtual void									ExecuteOperation(index_t id, DwarfViewOperationContext* pCtx);
};