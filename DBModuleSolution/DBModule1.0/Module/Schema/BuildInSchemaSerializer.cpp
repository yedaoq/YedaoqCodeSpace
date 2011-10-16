#include "mytype.h"
#include "BuildInSchemaSerializer.h"
#include "DBColumnSchema.h"
#include "..\DBModule.h"
#include "..\DBTableCollection.h"
#include "..\DBTable.h"
#include <boost\xpressive\xpressive.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\format.hpp>
#include <ios>
#include <sstream>

using namespace NSDBModule;
using namespace boost::xpressive;
using namespace boost::xpressive::regex_constants;

namespace NSDBModule
{
	struct BuildInSchemaSerializeObject
	{
		tregex				RegexTblBegin;
		tregex				RegexTblEnd;
		tregex				RegexColumn;
		tregex				RegexSchBegin;
		tregex				RegexSchEnd;
		tregex				RegexBlank;

		tformat				FormatTblBegin;
		tformat				FormatColumn;

		syntax_option_type	MatchOption;

		BuildInSchemaSerializeObject()
			: MatchOption(ignore_white_space | ECMAScript),
			RegexTblBegin(wsregex_compiler().compile(TEXT("^\\s*BEGIN_DBTABLE\\(\\s*(\\w+)\\s*\\)$"))),
			RegexTblEnd(wsregex_compiler().compile(  TEXT("^\\s*END_DBTABLE$"))),
			RegexColumn(wsregex_compiler().compile(  TEXT("^\\s*DBCOLUMN\\(\\s*\\w+\\s*,\\s*(\\w+)\\s*,\\s*(\\w+)\\s*,\\s*(\\w+)\\s*,\\s*([\\w\\\|]+)\\s*\\)$"))),
			RegexSchBegin(wsregex_compiler().compile(TEXT("^\\s*BEGIN_DBSCHEMA\\(\\s*(\\w+)\\s*,\\s*(\\w+)\\s*\\)$"))),
			RegexSchEnd(wsregex_compiler().compile(  TEXT("^\\s*END_DBSCHEMA$"))),
			RegexBlank(wsregex_compiler().compile(   TEXT("^\\s*$"))),
			FormatColumn(TEXT("DBCOLUMN(%-30s, %-30s, %-10s, 0x%08X, %s)")),
			FormatTblBegin(TEXT("BEGIN_DBTABLE(%s)"))
		{}
	};
}




CBuildInSchemaSerializer::CBuildInSchemaSerializer()
	: SerializeObject(new BuildInSchemaSerializeObject())
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
	tmatch match;

	module.Tables().Clear();	

	while(rows.MoveNext(row))
	{
		if(boost::xpressive::regex_match(row, match, SerializeObject->RegexTblBegin))
		{
			CDBTable* pTbl = module.Tables().Append(match[1].str(), &module, true);
			ReadTable(rows, pTbl->GetSchema());
		}
		else if(boost::xpressive::regex_match(row, match, SerializeObject->RegexBlank))
		{
			continue;
		}
		else if(boost::xpressive::regex_match(row, match, SerializeObject->RegexSchEnd))
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
	tmatch			match;
	tstring			row;

	schema.Clear(true);	

	while(rows.MoveNext(row))
	{
		if(boost::xpressive::regex_match(row,match, SerializeObject->RegexColumn))
		{
			col.Reset();
			col.Name = match[1].str();
			//col.Type = (EnumCppDataType)boost::lexical_cast<int>(match[2].str());
			col.Type = (EnumCppDataType)EnumEntityOfCppDataType()[match[2].str().c_str()].ValueEnum;
			tstringstream(match[4].str()) >> std::hex >> col.IndexMask;
			col.Flag = EnumEntityOfDBColumnSchemaFlag().ParseFlagEnum(match[5].str());

			col.ResetExternInfo();
			schema.AppendColumn(col);
		}
		else if(boost::xpressive::regex_match(row, match, SerializeObject->RegexTblEnd))
		{
			break;
		}
		else if(boost::xpressive::regex_match(row, match, SerializeObject->RegexBlank))
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
	tformat fmtTblBegin = SerializeObject->FormatTblBegin;
	tformat fmtCol = SerializeObject->FormatColumn;

	rows.push_back((fmtTblBegin%tbl.Name).str());
	
	std::auto_ptr<IEnumerator<DBColumnSchema>> cols(tbl.EnumColumn());

	DBColumnSchema col;
	while(cols->MoveNext(col))
	{
		col.ResetExternInfo();

		fmtCol
			%tbl.Name
			%col.Name
			%EnumEntityOfCppDataType()[col.Type].ValueStr
			%col.IndexMask
			%EnumEntityOfDBColumnSchemaFlag().FormatFlagEnum(col.Flag);

		rows.push_back(fmtCol.str());
	}

	rows.push_back(TEXT("END_DBTABLE\n"));

	return 1;
}