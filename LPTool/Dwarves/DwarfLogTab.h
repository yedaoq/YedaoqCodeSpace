#pragma once
#include "SideTab.h"
#include "Module/DMLNotifier/DMLEvent.h"

class CDwarfLogTab : public CListBox, public ISideTab
{
public:
	CDwarfLogTab(void);
	~CDwarfLogTab(void);

	virtual int  Initialize(CWnd* pParent, IDwarfViewInfo* pView) {return 1;};

	virtual bool IsRelatedToMainView() {return false;}
	virtual int  GetViewID() {return -1;}

	virtual int  GetValidityCounter(){return -1;}
	virtual int  SetValidityCounter(int) {return 1;};
	virtual int  ContentUpdate(DwarfViewOperationContext* pCtx) {return 1;}

	int BeginMonit(NSDBModule::CDBModule* db);
	int StopMonit();
	void DMLRing(NSDBModule::DMLEvent* e);

	tstring GetLogString(NSDBModule::DMLEvent* e);
	tstring GetLogRecordStr(const NSDBModule::IDBRecord* rec);

protected:
	NSDBModule::CDBModule*	DataBase;
	int						MonitorID;
};
