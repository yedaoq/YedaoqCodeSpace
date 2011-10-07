#pragma once

#include "mytype.h"
#include "Singlton.h"

interface IContext
{};

interface ITextFormatter
{
	virtual ~ITextFormatter(){}
	virtual tstring Format(const tstring& val, IContext* ctx) = 0;
	
};

interface ITextParser
{
	virtual ~ITextParser(){}
	virtual tstring Parse(const tstring& val, IContext* ctx) = 0;
};

interface ITextFormatSwitcher : public ITextFormatter, public ITextParser
{

};

class CTextFormatSwitcherNone : public ITextFormatSwitcher, public CSingleton<CTextFormatSwitcherNone>
{
public:
	virtual tstring Parse(const tstring& val, IContext* ctx) { return val; }
	virtual tstring Format(const tstring& val, IContext* ctx) { return val; }
};