#pragma once
#include "DwarfViewInfo.h"

class CDwarfViewOperationCollection
{
public:
	int Count() const {return Items; }

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
	CDwarfViewInfoBase(void);

	virtual int										GetViewID();

	virtual CDBTableViewColumnCollection&			GetViewColumnCollection() { return Columns; }

	virtual IEnumerator<IDwarfViewInfo*>*			EnumReleatedView() { return new_iterator_enumerator(ReleatedViews.begin(), ReleatedViews.end()); }

	virtual IEnumerator<IDBRecord>*					EnumRecord4RelatedView(IDBRecord& item) = 0;
	virtual IEnumerator<IDBRecord>*					EnumRecord() = 0;

	virtual IEnumerator<DwarfViewOperationItem>*	EnumOperation() { return new_iterator_enumerator(Operations.Items.begin(), Operations.Items.end()); }
	virtual void									ExecuteOperation(index_t id, DwarfViewOperationContext* pCtx);

protected:
	CDwarfViewOperationCollection	Operations;
	std::vector<IDwarfViewInfo*>	ReleatedViews;
	CDBTableViewColumnCollection	Columns;
	int								ViewID;
};
