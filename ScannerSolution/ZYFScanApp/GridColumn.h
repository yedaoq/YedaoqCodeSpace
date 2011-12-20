#pragma once

#include "ITextFormater.h"
#include "EditStyle.h"
#include "CellTemplate.h"
#include "CellFactory.h"
#include "CellFactoryProvider.h"

class CGridColumn
{
public:
	CGridColumn(const tstring& title = TEXT(""), int defaultsize = 0, const CCellTemplate& tpl = CCellTemplate::DefaultTemplate, DWORD state = 0)
		: StrTitle_(title), Width_(defaultsize), CellTemplate_(tpl), CellState_(state)
	{}

	virtual void			SetTitle(const tstring& val)		{ StrTitle_ = val; }
	virtual void			SetWidth(int val)					{ Width_ = val; }
	virtual void			SetCellTemplate(const CCellTemplate& val)	{ CellTemplate_ = val; }
	virtual int				SetCellState(DWORD val)					{ CellState_ = val; return 1; }

	virtual const tstring&			GetTitle() const			{ return StrTitle_; }
	virtual int						GetWidth() const			{ return Width_; }
	virtual const CCellTemplate&	GetCellTemplate() const		{ return CellTemplate_; }
	virtual	DWORD					GetCellState()				{ return CellState_; }

protected:
	tstring					StrTitle_;
	int						Width_;
	CCellTemplate			CellTemplate_;
	DWORD					CellState_;
};

