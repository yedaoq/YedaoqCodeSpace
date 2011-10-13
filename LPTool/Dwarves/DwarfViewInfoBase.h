#pragma once
#include "DwarfViewInfo.h"

class CDwarfViewOperationCollection
{
public:
	int Count() const {return Items; }

	int Append(tstring name, DelegateOperation op)
	{
		Items.push_back()
	}

public:
	std::vector<DwarfViewOperationItem> Items;
};

class CDwarfViewInfoBase : public IDwarfViewInfo
{
public:
	CDwarfViewInfoBase(void);
	~CDwarfViewInfoBase(void);
};
