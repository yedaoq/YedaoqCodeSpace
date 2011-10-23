#pragma once
#include <View\DBColumnViewInfo.h>

class CDwarfViewInfoDBTblBase : public CDwarfViewInfoBase
{
public:
	CDwarfViewInfoDBTblBase(CDBModule* module, index_t id)
		: DBModule(module), ViewID(id)
	{}

	~CDwarfViewInfoDBTblBase(void);

	virtual int	GetViewID() { return ViewID; }

	virtual int Initialize();
	virtual int InitializeReleatedViews();
	virtual int InitializeViewColumns();
	virtual int InitializeOperations();

	virtual CDBColumnViewInfo GenerateColumnViewFromSchema(const DBColumnSchema& col);

protected:	
	CDBModule* DBModule;
};
