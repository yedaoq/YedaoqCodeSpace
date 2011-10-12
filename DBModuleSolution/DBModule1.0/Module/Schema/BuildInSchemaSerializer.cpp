#include "BuildInSchemaSerializer.h"
#include "DBColumnSchema.h"
#include "..\DBModule.h"
#include "..\DBTableCollection.h"
#include "..\DBTable.h"
#include "mytype.h"
#include <regex>
#include <boost\lexical_cast.hpp>
#include <boost\format.hpp>
#include <ios>
#include <sstream>

using namespace NSDBModule;

typedef boost::basic_format<tchar> tformat;
typedef std::tr1::match_results<tstring::const_iterator> tmatch_results;

CBuildInSchemaSerializer::CBuildInSchemaSerializer()
	: RegexTblBegin(TEXT("^\\s*BEGIN_DBTABLE\\((\\w+)\\)\\s*$")),
	  RegexTblEnd(TEXT("^\\s*END_DBTABLE\\s*$")),
	  RegexColumn(TEXT("^\\s*DBCOLUMN\\(\\s*\\w+\\s*,\\s*(\\w+)\\s*,\\s*(\\w+),\\s*(\\w+),\\s*(\\w+)\\)\\s*$")),
	  RegexSchBegin(TEXT("^\\s*BEGIN_DBSCHEMA\\((\\w+),(\\w+)\\)\\s*$")),
	  RegexSchEnd(TEXT("^\\s*END_DBSCHEMA\\s*$")),
	  RegexBlank(TEXT("^\\s*$"))
{}


bool CBuildInSchemaSerializer::Read(IEnumerator<tstring>& rows, CDBModule& module)
{
	bool bRet = true;

	if(!ReadAllTable(rows, module))
	{
		bRet = false;
	}

	return bRet;
}

bool CBuildInSchemaSerializer::ReadAllTable(IEnumerator<tstring>& rows, CDBModule& module)
{
	bool bRet = false;
	tstring row;
	tmatch_results match;

	module.Tables().Clear();	

	while(rows.MoveNext(row))
	{
		if(std::tr1::regex_match(row,match, RegexTblBegin))
		{
			CDBTable* pTbl = module.Tables().Append(match[1].str(), &module, true);
			ReadTable(rows, pTbl->GetSchema());
		}
		else if(std::tr1::regex_match(row, match, RegexBlank))
		{
			continue;
		}
		else if(std::tr1::regex_match(row, match, RegexSchEnd))
		{
			continue;
		}
		else
		{
			_ASSERT(false);
			return false;
		}
	}

	return true;
}

bool CBuildInSchemaSerializer::ReadTable(IEnumerator<tstring>& rows, CDBTableSchema& schema)
{
	DBColumnSchema	col;
	tmatch_results	match;
	tstring			row;

	schema.Clear(true);	

	while(rows.MoveNext(row))
	{
		if(std::tr1::regex_match(row,match, RegexColumn))
		{
			col.Reset();
			col.Name = match[1].str();
			col.Type = (EnumCppDataType)boost::lexical_cast<int>(match[2].str());
			tstringstream(match[3].str()) >> std::hex >> col.Flag;
			tstringstream(match[4].str()) >> std::hex >> col.UniqueMask;

			col.ResetExternInfo();
			schema.AppendColumn(col);
		}
		else if(std::tr1::regex_match(row, match, RegexTblEnd))
		{
			break;
		}
		else if(std::tr1::regex_match(row, match, RegexBlank))
		{
			continue;
		}
		else
		{
			_ASSERT(false);
		}
	}

	return true;
}

bool CBuildInSchemaSerializer::Write(CDBModule& module, FileRowCollection& rows)
{
	bool bRet = false;

	if(!WriteAllTable(module, rows))
	{
		return bRet;
	}
	bRet = true;
	return bRet;
}

bool CBuildInSchemaSerializer::WriteAllTable(CDBModule& module, FileRowCollection& rows)
{
	std::auto_ptr<DBTableEnumerator> pEnumTbl(module.Tables().Enum());
	while(pEnumTbl->MoveNext())
	{
		WriteTable(pEnumTbl->Current()->GetSchema(), rows);
	}

	return true;
}

bool CBuildInSchemaSerializer::WriteTable(const CDBTableSchema& tbl, FileRowCollection& rows)
{
	//tformat fmtCol(TEXT("DBCOLUMN(%s,%d,%u,%u)"));
	tformat fmtCol(TEXT("DBCOLUMN(%-30s, %-30s, %d, 0x%08X, 0x%08X)"));

	rows.push_back((tformat(TEXT("BEGIN_DBTABLE(%s)"))%tbl.Name).str());
	
	std::auto_ptr<IEnumerator<DBColumnSchema>> cols(tbl.EnumColumn());

	DBColumnSchema col;
	while(cols->MoveNext(col))
	{
		col.ResetExternInfo();
		rows.push_back((fmtCol%tbl.Name%col.Name%col.Type%col.Flag%col.UniqueMask).str());
	}

	rows.push_back(TEXT("END_DBTABLE\n"));

	return 1;
}