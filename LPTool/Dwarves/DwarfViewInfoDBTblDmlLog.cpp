#include "StdAfx.h"
#include "DwarfViewInfoDBTblDmlLog.h"
#include <Module\DMLNotifier\EnumDMLCommand.h>
#include <mytype.h>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/xpressive/xpressive.hpp>

#define BOOST_DATE_TIME_SOURCE
#include <libs/date_time/src/gregorian/greg_names.hpp>
#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/gregorian/greg_month.cpp>
#include <libs/date_time/src/gregorian/greg_weekday.cpp>
#include <libs/date_time/src/gregorian/gregorian_types.cpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

//template<>
CDwarfViewInfoDBTable<TBL_DmlLog>::CDwarfViewInfoDBTable(void)
	: CDwarfViewInfoDBTblBase(&CDBModuleLP::GetInstance(), TBL_DmlLog)
{
	CDBRecordAuto recVer;
	if(DBModule->Tables()[TBL_Option]->Find(COL_Option_Id, TEXT("1"), recVer) > 0)
	{
		DBVersion = recVer.GetField(COL_Option_Value);
	}

	MonitorID = DBModule->DMLNotifier().AppendMonitor(
		EnumDMLCommand::DMLDelete | EnumDMLCommand::DMLInsert | EnumDMLCommand::DMLUpdate, -1,
		DelegateDMLMonit(this, &CDwarfViewInfoDBTable<TBL_DmlLog>::DMLRing));
}

int CDwarfViewInfoDBTable<TBL_DmlLog>::InitializeOperations()
{
	__super::InitializeOperations();

	this->Operations.Append(TEXT("�°汾"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTable<TBL_DmlLog>::OnNewVersion));

	return 1;
}

void CDwarfViewInfoDBTable<TBL_DmlLog>::OnNewVersion( DwarfViewOperationContext* pCtx )
{
	tregex reg = tregex::compile(TEXT("^(\\d+).(\\d+).(\\d+).(\\d+)$"));
	tmatch match;

	int VerMain, VerMonth, VerDay, VerSerial;
	int CurMonth, CurDay;

	if(!boost::xpressive::regex_match(DBVersion, match, reg))
	{
		return;
	}

	VerMain = boost::lexical_cast<int>(match[1].str());
	VerMonth = boost::lexical_cast<int>(match[2].str());
	VerDay = boost::lexical_cast<int>(match[3].str());
	VerSerial = boost::lexical_cast<int>(match[4].str());

	boost::gregorian::date curDate = boost::gregorian::day_clock::local_day();

	if(VerDay != curDate.day() || VerMonth != curDate.month())
	{
		VerDay = curDate.day();
		VerMonth = curDate.month();
		VerSerial = 1;
	}
	else
	{
		++VerSerial;
	}

	tformat fmt(TEXT("%d.%d.%d.%d"));
	fmt%VerMain%VerMonth%VerDay%VerSerial;
	DBVersion = fmt.str();

	CDBRecordAuto recVer;
	if(DBModule->Tables()[TBL_Option]->Find(COL_Option_Id, TEXT("1"), recVer) > 0)
	{
		recVer.SetField(COL_Option_Value, DBVersion);
	}

	DBModule->Tables()[TBL_Option]->Update(recVer, recVer);
}

//template<>
void CDwarfViewInfoDBTable<TBL_DmlLog>::DMLRing( DMLEvent* e )
{
	if(!e || e->TableID == TBL_DmlLog) return; 

	CDBRecordBase rec = DBModule->Tables()[TBL_DmlLog]->RecordTemplate();

	rec.SetField(COL_DmlLog_Command, boost::lexical_cast<tstring>(e->Command));
	rec.SetField(COL_DmlLog_TableName, DBModule->Tables()[e->TableID]->GetName());
	rec.SetField(COL_DmlLog_Dbver, DBVersion);
	rec.SetField(COL_DmlLog_RecordOrigin, GetLogRecordStr(e->RecordOrigin));
	rec.SetField(COL_DmlLog_RecordFresh, GetLogRecordStr(e->RecordFresh));
	rec.SetField(COL_DmlLog_User, TEXT("δ֪"));
	rec.SetField(COL_DmlLog_Datetime, boost::posix_time::to_iso_wstring(boost::posix_time::second_clock::local_time()));

	DBModule->Tables()[TBL_DmlLog]->Insert(rec);
	CGlobalData::GetLogTab()->Append(rec);
	
	CDwarfView* pLogView = CGlobalData::GetViewByID(TBL_DmlLog);
	if(pLogView)
	{
		pLogView->AddRecord(rec);
	}
}

tstring CDwarfViewInfoDBTable<TBL_DmlLog>::GetLogRecordStr( const IDBRecord* rec )
{
	tstring strRet = TEXT("0");

	if(rec)
	{
		strRet[0] = '{';
		for (int i = 0; i < rec->GetFieldCount(); ++i)
		{
			strRet.append(rec->GetField(i));
			strRet.append(TEXT(", "));
		}

		*(strRet.end() - 2) = '}';
	}

	return strRet;
}
