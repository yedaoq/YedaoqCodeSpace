#pragma once

#include <mytype.h>

namespace NSDBModule
{
	interface ITextFormatSwitcher;
	interface IEditStyle;

	class CDBColumnViewInfo
	{
	public:
		CDBColumnViewInfo(const tstring& title, ITextFormatSwitcher* view = 0, IEditStyle* editStyle = 0, int defaultsize = 0)
			: StrTitle_(title), View_(view), EditStyle_(editStyle), DefaultWidth_(defaultsize)
		{}

		virtual tstring&	SetTitle(const tstring& val)			{ StrTitle_ = val; }
		virtual int			SetDefaultWidth(int val)				{ DefaultWidth_ = val; }
		virtual int			SetTextFormat(ITextFormatSwitcher* val)	{ View_ = val; }
		virtual int			SetEditStyle(IEditStyle* val)			{ EditStyle_ = val; }

		virtual const tstring&			GetTitle()					{ return StrTitle_; }
		virtual int						GetDefaultWidth()			{ return DefaultWidth_; }
		virtual ITextFormatSwitcher*	GetTextFormat()				{ return View_; }
		virtual IEditStyle*				GetEditStyle()				{ return EditStyle_; }

	protected:
		tstring					StrTitle_;
		int						DefaultWidth_;
		ITextFormatSwitcher*	View_;
		IEditStyle*				EditStyle_;
	};
}