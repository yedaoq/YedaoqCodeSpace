#pragma once

#include "ITextFormater.h"
#include "EditStyle.h"
#include "CellTemplate.h"
#include "CellFactory.h"
#include "CellFactoryProvider.h"

class CGridColumn
{
public:
	CGridColumn(const tstring& title = TEXT(""), int defaultsize = 0, const CCellTemplate& tpl = CCellTemplate::DefaultTemplate)
		: StrTitle_(title), Width_(defaultsize), CellTemplate_(tpl)
	{}

	virtual void			SetTitle(const tstring& val)		{ StrTitle_ = val; }
	virtual void			SetWidth(int val)					{ Width_ = val; }
	virtual void			SetCellTemplate(const CCellTemplate& val)	{ CellTemplate_ = val; }

	virtual const tstring&			GetTitle() const			{ return StrTitle_; }
	virtual int						GetWidth() const			{ return Width_; }
	virtual const CCellTemplate&	GetCellTemplate() const		{ return CellTemplate_; }

protected:
	tstring					StrTitle_;
	int						Width_;
	CCellTemplate			CellTemplate_;
};

