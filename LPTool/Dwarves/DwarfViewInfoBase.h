#pragma once
#include "DwarfViewInfo.h"
#include "DBTableViewer4GridCtrl.h"


class CDwarfViewOperationCollection
{
public:
	int Count() const {return Items.size(); }

	int GetMaxOperationID() const
	{
		int iRet = -1;
		for (int i = 0; i < Items.size(); ++i)
		{
			if(Items[i].ID > iRet) iRet = Items[i].ID;
		}
		return iRet;
	}

	int Exist(int id) const
	{
		for (int i = 0; i < Items.size(); ++i)
		{
			if(Items[i].ID == id) return true;
		}
		return false;
	}

	int Append(tstring name, DelegateOperation op)
	{
		Items.push_back(DwarfViewOperationItem(GetMaxOperationID() + 1, name, op));
		return 1;
	}

	int Append(int id, tstring name, DelegateOperation op)
	{
		if(!Exist(id))
		{
			Items.push_back(DwarfViewOperationItem(id, name, op));
		}
		else
		{
			_ASSERT(false);
		}
	}

public:
	std::vector<DwarfViewOperationItem> Items;
};

class CDwarfViewInfoBase : public IDwarfViewInfo
{
public:
	typedef std::vector<CDBColumnViewInfo>	DBColumnViewInfoCollection;
	typedef std::vector<IDwarfViewInfo*>	RelatedViewCollection;

	CDwarfViewInfoBase(int id)
		: ViewID(id), InitializeFlag(false)
	{}

	virtual int										GetViewID() { return ViewID; };

	virtual int										Initialize() { InitializeFlag = true; return 1; }

	virtual CDBTableViewColumnCollection&			GetViewColumnCollection();// { return ViewColumns; }

	virtual IEnumerator<IDwarfViewInfo*>*			EnumReleatedView();// { return new_iterator_enumerator(ReleatedViews.begin(), ReleatedViews.end()); }

	virtual IEnumerator<IDBRecord>*					EnumRecordAsRelatedView(DwarfViewOperationContext* pCtx) = 0;
	virtual IEnumerator<IDBRecord>*					EnumRecord() = 0;

	virtual IEnumerator<DwarfViewOperationItem>*	EnumOperation();// { return new_iterator_enumerator(Operations.Items.begin(), Operations.Items.end()); }
	virtual void									ExecuteOperation(index_t id, DwarfViewOperationContext* pCtx);

	virtual bool									Initialized() const { return InitializeFlag; }

protected:
	CDwarfViewOperationCollection	Operations;			// 视图操作集合
	RelatedViewCollection			ReleatedViews;		// 关联视图集合
	CDBTableViewColumnCollection	ViewColumns;		// 视图中的列集
	DBColumnViewInfoCollection		ColumnViewInfos;	// 列的展示信息的集合
	int								ViewID;				// 视图的ID
	bool							InitializeFlag;
};

CDBColumnViewInfo& GetGridCol4Select();