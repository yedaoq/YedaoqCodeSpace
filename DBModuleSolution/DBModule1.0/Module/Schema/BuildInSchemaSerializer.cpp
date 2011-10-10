#include "BuildInSchemaSerializer.h"
#include "DBModule.h"
#include "DBTableCollection.h"
#include "DBTable.h"
#include "DBColumnSchema.h"
#include "mytype.h"
#include <regex>
#include <boost\lexical_cast.hpp>
#include <boost\format.hpp>

using namespace NSDBModule;

typedef boost::basic_format<tchar> tformat;
typedef std::tr1::match_results<tstring::const_iterator> tmatch_results;

CBuildInSchemaSerializer::CBuildInSchemaSerializer()
	: RegexTblBegin(TEXT("^\s*BEGIN_DBTABLE\((\w+)\)\s*$")),
	  RegexTblEnd(TEXT("^\s*END_DBTABLE\s*$")),
	  RegexColumn(TEXT("^\s*DBCOLUMN\((\w+),(\w+),(\w+),(\w+)\)\s*$")),
	  RegexSchBegin(TEXT("^\s*BEGIN_DBSCHEMA\((\w+),(\w+)\)\s*$")),
	  RegexSchEnd(TEXT("^\s*END_DBSCHEMA\s*$")),
	  RegexBlank(TEXT("^\s*$"))
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

	module.Tables().Clear();

	tmatch_results match;

	while(rows.MoveNext())
	{
		if(std::tr1::regex_match(rows.Current(),match, RegexTblBegin))
		{
			CDBTable* pTbl = module.Tables().Append(match[0].str(), &module, true);
			ReadTable(rows, pTbl->GetSchema());
		}
		else if(std::tr1::regex_match(rows.Current(), match, RegexBlank))
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
	schema.Clear(true);

	//tmatch match;
	tmatch_results match;

	while(rows.MoveNext())
	{
		if(std::tr1::regex_match(rows.Current(),match, RegexColumn))
		{
			DBColumnSchema col;
			col.Name = match[0].str();
			col.Type = (EnumCppDataType)boost::lexical_cast<int>(match[1].str());
			col.SetFlag(boost::lexical_cast<flag_t>(match[2].str()), true);
			col.UniqueMask = boost::lexical_cast<flag_t>(match[3].str());

			schema.AppendColumn(col);
		}
		else if(std::tr1::regex_match(rows.Current(), match, RegexBlank))
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
	tformat fmtCol(TEXT("DBCOLUMN(%s,%d,%u,%u)"));

	rows.push_back((tformat(TEXT("BEGIN_DBTABLE(%s)"))%tbl.Name).str());
	
	IEnumerator<DBColumnSchema>& cols = tbl.EnumColumn();

	DBColumnSchema col;
	while(cols.MoveNext(col))
	{
		rows.push_back((fmtCol%col.Name%col.Type%col.Flag%col.UniqueMask).str());
	}

	rows.push_back(TEXT("END_DBTABLE"));

	return 1;
}