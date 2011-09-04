#include "DBDataTypeParser.h"
#include "regex"
#include "mytype.h"
#include "DBDataTypeCommon.h"


using namespace NSDBModule;

IDBDataType* CDBDataTypeDecimalParser::Parse(const tstring& type)
{
	static tregex reg(TEXT("^decimal\((\d+),(\d+)\)$"));
	tmatch match;
	if(std::tr1::regex_match(type, match, reg))
	{
		index_t len = match[0].str();
		index_t precision = match[1].str();

		return new CDBDataTypeDecimal(len, precision);
	}

	return 0;
}

IDBDataType* CDBDataTypeStringParser::Parse(const tstring& type)
{
	static tregex reg(TEXT("^(char|nchar|varchar|nvarchar)\((\d+)\)$"));
	tmatch match;
	if(std::tr1::regex_match(type, match, reg))
	{
		tstring name = match[0].str();
		index_t len =  match[1].str(); 
		bool	bUnicode = (name[0] == 'n');
		bool	bFixedWidth = (name.substr(bUnicode? 1 : 0, 3) == TEXT("var"));
		return new CDBDataTypeString(len, bFixedWidth, bUnicode);
	}

	return 0;
}