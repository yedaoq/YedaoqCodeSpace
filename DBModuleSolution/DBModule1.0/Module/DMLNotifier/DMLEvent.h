#pragma once
#include <FastDelegate.h>
#include "mytype.h"
#include "EnumDMLCommand.h"

namespace NSDBModule
{
	class CDBModule;
	interface IDBRecord;

	struct DMLEvent
	{
		CDBModule*			Database;
		EnumDMLCommand		Command;
		int					TableID;
		const IDBRecord*	RecordOrigin;
		const IDBRecord*	RecordFresh;
		bool				FlagCancel;

		DMLEvent(CDBModule* db, int tblID)
			: Database(db), TableID(tblID)
		{}
	};

	typedef fastdelegate::FastDelegate1<DMLEvent*> DelegateDMLMonit;

}