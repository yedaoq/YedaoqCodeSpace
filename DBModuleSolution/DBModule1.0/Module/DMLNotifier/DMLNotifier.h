#pragma once

#include "mytype.h"
#include <map>
#include "EnumDMLCommand.h"
#include "DMLEvent.h"

namespace NSDBModule
{
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

}