#include "StdAfx.h"
#include "DwarfLogTab.h"
#include "Module\DBModule.h"
#include "Module\DBRecord.h"
#include "Module\DMLNotifier\DMLNotifier.h"
#include <mytype.h>
#include "LocalPlayerObjEnum.h"

using namespace NSDBModule;

CDwarfLogTab::CDwarfLogTab(void)
	: MonitorID(-1), DataBase(0)
{}

CDwarfLogTab::~CDwarfLogTab(void)
{
	//StopMonit();
}

//int CDwarfLogTab::BeginMonit( NSDBModule::CDBModule* db )
//{
//	if(!db)
//	{
//		ASSERT(false);
//		return -1;
//	}
//
//	DataBase = db;
//	MonitorID = DataBase->DMLNotifier().AppendMonitor(
//		EnumDMLCommand::DMLDelete | EnumDMLCommand::DMLInsert | EnumDMLCommand::DMLUpdate, -1,
//		DelegateDMLMonit(this, &CDwarfLogTab::DMLRing));
//
//	this->ResetContent();
//
//	return 1;
//}
//
//int CDwarfLogTab::StopMonit()
//{
//	if(DataBase && MonitorID != -1)
//	{
//		DataBase->DMLNotifier().RemoveMonitor(MonitorID);
//	}
//	DataBase = 0;
//	MonitorID = -1;
//
//	return 1;
//}
//
//void CDwarfLogTab::DMLRing( DMLEvent* e )
//{
//	this->InsertString(-1, GetLogString(e).c_str());
//}
//
//tstring CDwarfLogTab::GetLogString( DMLEvent* e)
//{
//	tstring strRet;
//	if(!e)
//	{
//		return strRet;
//	}
//
//	strRet.append(EnumEntityOfDMLCommand()[e->Command].ValueDescription);
//	strRet.append(TEXT(" : "));
//	strRet.append(e->RecordOrigin ? GetLogRecordStr(e->RecordOrigin) : TEXT("0"));
//	strRet.append(TEXT(" -> "));
//	strRet.append(e->RecordFresh ? GetLogRecordStr(e->RecordFresh) : TEXT("0"));
//	return strRet;
//}
//
//tstring CDwarfLogTab::GetLogRecordStr( const IDBRecord* rec )
//{
//	ASSERT(rec);
//	tstring strRet = TEXT("{");
//
//	for (int i = 0; i < rec->GetFieldCount(); ++i)
//	{
//		strRet.append(rec->GetField(i));
//		strRet.append(TEXT(", "));
//	}
//
//	*(strRet.end() - 2) = '}';
//
//	return strRet;
//}

int CDwarfLogTab::Append( const IDBRecord& recLog )
{
	tstring strRet;

	strRet.append(recLog.GetField(COL_DmlLog_Command));
	strRet.append(TEXT(" "));
	strRet.append(recLog.GetField(COL_DmlLog_TableName));
	strRet.append(TEXT(" : "));
	strRet.append(recLog.GetField(COL_DmlLog_RecordOrigin));
	strRet.append(TEXT(" -> "));
	strRet.append(recLog.GetField(COL_DmlLog_RecordFresh));
	
	return this->InsertString(-1, strRet.c_str());
}

