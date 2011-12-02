#pragma once

#include "mytype.h"
#include <FastDelegate.h>

enum EnumDMLCommand
{
	Unknow = 0;
	Insert = 1;
	Update = 2;
	Delete = 4;
};

struct DMLEvent
{
	NSDBModule::CDBModule*			Database;
	EnumDMLCommand					Command;
	int								TableID;
	const NSDBModule::IDBRecord*	RecordOrigin;
	const NSDBModule::IDBRecord*	RecordFresh;
	bool							FlagCancel;
};

typedef fastdelegate::FastDelegate1<DMLEvent*> DelegateDMLMonit;

//typedef void DelegateDMLMonit(DMLEvent*);

class CDMLMonitor
{
public:
	void Ring(DMLEvent* e);

	CDMLMonitor(int tbl, flag_t cmd, const DelegateDMLMonit& delegate)
		: Table2Monit(tbl), Command2Monit(cmd), Delegate(delegate)
	{}

public:
	int					Table2Monit;
	flag_t				Command2Monit;
	DelegateDMLMonit	Delegate;
};

class CDMLNotifier
{
	typedef std::map<int, CDMLMonitor> MonitorMap;

public:
	void	Dispatch(DMLEvent* e);

	int		AppendMonitor(flag_t cmd, int tbl, DelegateDMLMonit cb);
	void	RemoveMonitor(int id);

protected:
	MonitorMap	Monitors;
	int			MonitorSN;
};