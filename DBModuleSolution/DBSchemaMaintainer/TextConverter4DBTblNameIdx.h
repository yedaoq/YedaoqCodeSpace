#pragma once

#include "Module/DBTableCollection.h"

class CTextConverter4DBTblNameIdx : public ITextFormatSwitcher
{
public:
	CTextConverter4DBTblNameIdx()
		: TablesPtr(0)
	{}

	CTextConverter4DBTblNameIdx(CDBTableCollection* pTables)
		: TablesPtr(pTables)
	{}

	virtual tstring Parse(const tstring& val, IContext* ctx)
	{ 
		return boost::lexical_cast<tstring>(TablesPtr->IndexOf(val)); 
	}

	virtual tstring Format(const tstring& val, IContext* ctx) 
	{ 
		int idx = boost::lexical_cast<int>(val.c_str());
		if(idx >= 0 && idx < TablesPtr->Count())
		{
			return (*TablesPtr)[idx]->GetName(); 
		}

		return tstring();
	}
	
protected:
	NSDBModule::CDBTableCollection* TablesPtr;
};