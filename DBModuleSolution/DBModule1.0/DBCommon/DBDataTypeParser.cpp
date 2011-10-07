#include "DBDataTypeParser.h"
#include "regex"
#include "mytype.h"
#include "DBDataTypeCommon.h"
#include "boost/lexical_cast.hpp"
//#include "boost/format.hpp"

using namespace NSDBModule;

IDBDataType* CDBDataTypeDecimalParser::Parse(const tstring& type) const
{
	static tregex reg(TEXT("^decimal\((\d+),(\d+)\)$"));
	tmatch match;
	if(std::tr1::regex_match(type.c_str(), match, reg))
	{
		index_t length = boost::lexical_cast<index_t>(match[0].str());
		index_t precision = boost::lexical_cast<index_t>(match[1].str());

		return new CDBDataTypeDecimal(length, precision);
	}

	return 0;
}

IDBDataType* CDBDataTypeStringParser::Parse(const tstring& type) const
{
	static tregex reg(TEXT("^(char|nchar|varchar|nvarchar)\((\d+)\)$"));
	tmatch match;
	if(std::tr1::regex_match(type.c_str(), match, reg))
	{
		tstring name = match[0].str();
		index_t len =  boost::lexical_cast<index_t>(match[1].str());
		bool	bUnicode = (name[0] == 'n');
		bool	bFixedWidth = (name.substr(bUnicode? 1 : 0, 3) == TEXT("var"));
		return new CDBDataTypeString(len, bFixedWidth, bUnicode);
	}

	return 0;
}