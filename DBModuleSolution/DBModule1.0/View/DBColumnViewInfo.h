#pragma once

#include <mytype.h>

interface ITextFormatSwitcher;
interface IEditStyle;

namespace NSDBModule
{
	class CDBColumnViewInfo
	{
	public:
		CDBColumnViewInfo() {}

		CDBColumnViewInfo(const tstring& title, ITextFormatSwitcher* view = 0, IEditStyle* editStyle = 0, int defaultsize = 0, bool readonly = false)
			: StrTitle_(title), View_(view), EditStyle_(editStyle), DefaultWidth_(defaultsize), ReadOnly_(readonly)
		{}

		virtual int			SetTitle(const tstring& val)			{ StrTitle_ = val; return 1; }
		virtual int			SetDefaultWidth(int val)				{ DefaultWidth_ = val; return 1; }
		virtual int			SetTextFormat(ITextFormatSwitcher* val)	{ View_ = val; return 1; }
		virtual int			SetEditStyle(IEditStyle* val)			{ EditStyle_ = val; return 1; }
		virtual int			SetReadOnly(bool val)					{ ReadOnly_ = val; return 1; }

		virtual const tstring&			GetTitle()					{ return StrTitle_; }
		virtual int						GetDefaultWidth()			{ return DefaultWidth_; }
		virtual ITextFormatSwitcher*	GetTextFormat()				{ return View_; }
		virtual IEditStyle*				GetEditStyle()				{ return EditStyle_; }
		virtual	bool					GetReadOnly()				{ return ReadOnly_; }

	protected:
		tstring					StrTitle_;
		int						DefaultWidth_;
		ITextFormatSwitcher*	View_;
		IEditStyle*				EditStyle_;
		bool					ReadOnly_;
	};
}