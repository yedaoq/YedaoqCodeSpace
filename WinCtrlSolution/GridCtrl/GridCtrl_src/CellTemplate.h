#pragma once
#include "mytype.h"
//#include "ITextFormater.h"
//#include "EditStyle.h"

class CGridCellBase;
interface ICellFactory;
interface ITextFormatSwitcher;
interface IEditStyle;

// 
class CCellTemplate
{
public:
	CCellTemplate(ITextFormatSwitcher* fmt, IEditStyle *editStyle, ICellFactory* factory = 0);

	virtual ITextFormatSwitcher*	GetTextFormat() const		{ return FormatSwitcher_; }
	virtual IEditStyle*				GetEditStyle() const		{ return EditStyle_; }
	virtual ICellFactory*			GetCellFactory() const		{ return CellFactory_; }

	virtual CGridCellBase*	CreateCell() const;
	virtual CGridCellBase*	CreateCell(tstring val) const;
	
protected:

	ITextFormatSwitcher*	FormatSwitcher_;
	IEditStyle*				EditStyle_;
	ICellFactory*			CellFactory_;

public:
	static CCellTemplate DefaultTemplate;
};
