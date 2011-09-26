#pragma once

#include "Layout.h"
#include <list>

namespace NSYedaoqLayout
{
	class CFlowLayout : ILayout
	{
	public:
		typedef std::list<ILayout*> ItemCollection;

		CFlowLayout(EnumLayoutDirection dir)
			: Direction_(dir)
		{}

		CFlowLayout(EnumLayoutDirection dir, ItemCollection items, const ResizeInfo& resizeInfo, const AnchorInfo& anchorInfo)
			: Direction_(dir), Items_(items), ResizeInfo_(resizeInfo), AnchorInfo_(anchorInfo)
		{}

		virtual ILayout*	Clone()						{ return new CFlowLayout(Direction_, Items_, ResizeInfo_, AnchorInfo_); }

		EnumLayoutDirection	GetDirection()				{ return Direction_; }
		ResizeInfo&			GetResizeInfo()				{ return ResizeInfo_; }
		AnchorInfo&			GetAnchorInfo()				{ return AnchorInfo_; }

		ILayout*			AddItem(HWND hWnd)			{ Items_.push_back(new CCtrlLayout(hWnd)); }
		ILayout*			AddItem(const ILayout& item){ Items_.push_back(item.Clone()); }

		virtual LayoutSize	AreaAdapt(const LayoutSize& lsContainer);
		virtual int			Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea);

	protected:
		LayoutSize			SizeAdapt(const LayoutSize& lsContainer);

	protected:
		EnumLayoutDirection Direction_;
		ResizeInfo			ResizeInfo_;
		AnchorInfo			AnchorInfo_;

		std::list<ILayout*>	Items_;
	};
}
