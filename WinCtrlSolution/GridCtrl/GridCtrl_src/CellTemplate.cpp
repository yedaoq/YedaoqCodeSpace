#include "stdafx.h"
#include "ITextFormater.h"
#include "EditStyle.h"

CCellTemplate	CCellTemplate::DefaultTemplate(
	&CTextFormatSwitcherNone::GetInstance(), 
	&CEditStyleNone::GetInstance(), 
	&CCellFactoryProvider::DefaultCellFactory);

// the cell template for CGridCell
CCellTemplate::CCellTemplate(ITextFormatSwitcher* fmt, IEditStyle *editStyle, ICellFactory* factory)
	: FormatSwitcher_(fmt), EditStyle_(editStyle), CellFactory_(factory)
{
	if(!GetCellFactory())
	{
		if(dynamic_cast<CEditStyleOptional*>(GetEditStyle()))
		{
			CellFactory_ = &CCellFactoryProvider::ComboBoxCellFactory;
		}
		else if(dynamic_cast<CEditStyleBool*>(GetEditStyle()))
		{
			CellFactory_ = &CCellFactoryProvider::CheckBoxCellFactory;
		}
		else if(dynamic_cast<CEditStyleLink*>(GetEditStyle()))
		{
			CellFactory_ = &CCellFactoryProvider::LinkCellFactory;
		}
		else
		{
			CellFactory_ = &CCellFactoryProvider::DefaultCellFactory;
		}
	}
}

// create a new cell
CGridCellBase *CCellTemplate::CreateCell() const
{
	ASSERT(GetCellFactory());
	CGridCellBase *cell = GetCellFactory()->CreateCell();
	cell->SetTemplate(const_cast<CCellTemplate*>(this));
	return cell;
}

CGridCellBase *CCellTemplate::CreateCell(tstring val) const
{
	CGridCellBase *cell = CreateCell();
	//cell->SetValue(val.c_str());

	//格式化并赋值

	throw std::exception();

	return cell;
}

