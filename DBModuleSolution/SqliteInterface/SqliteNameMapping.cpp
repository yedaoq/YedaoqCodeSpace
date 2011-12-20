#include "stdafx.h"
#include "SqliteNameMapping.h"
#include "mytype.h"
#include "boost\xpressive\xpressive.hpp"
#include <boost\algorithm\string\case_conv.hpp>

using namespace NSDBModule;

struct ReplaceToUpper
{
	tstring operator()(boost::xpressive::wsmatch const &m) const
	{
		return boost::to_upper_copy(m[1].str());
	}
};

struct ReplaceToLower
{
	tstring operator()(boost::xpressive::wsmatch const &m) const
	{
		return boost::to_lower_copy(m[1].str());
	}
};

tstring CSqliteNameMapping::FromDBName(const tstring& dbName, const ContextDBNameMapping& context)
{
	static boost::xpressive::wsregex reg = boost::xpressive::wsregex_compiler().compile(TEXT("(?:^|_)([A-Za-z0-9])"));
	//static boost::xpressive::wsregex reg(TEXT("(?:^|_)([A-Za-z0-9])"));
	return boost::xpressive::regex_replace(dbName, reg, ReplaceToUpper());
}

tstring CSqliteNameMapping::ToDBName(const tstring& name, const ContextDBNameMapping& context)
{
	static boost::xpressive::wsregex reg = boost::xpressive::wsregex_compiler().compile(TEXT("([A-Z]+)"));
	//static boost::xpressive::wsregex reg(TEXT("([A-Z]+)"));
	return boost::xpressive::regex_replace(name, reg, ReplaceToLower());
}