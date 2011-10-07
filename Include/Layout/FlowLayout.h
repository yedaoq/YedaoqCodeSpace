#pragma once

#include "Layout.h"
#include <list>

namespace NSYedaoqLayout
{
	class CCtrlLayout;
	class CZoomBlankLayout;

	class CFlowLayout : public ILayout
	{
	public:
		typedef std::list<ILayout*> ItemCollection;

		CFlowLayout(EnumLayoutDirection dir)
			: Direction_(dir), ResizeInfo_(EnumResizeMode::Auto, -1)
		{}

		CFlowLayout(EnumLayoutDirection dir, const ResizeInfo& resizeInfo)
			: Direction_(dir), ResizeInfo_(resizeInfo)
		{}

		CFlowLayout(EnumLayoutDirection dir, ItemCollection items, const ResizeInfo& resizeInfo)
			: Direction_(dir), Items_(items), ResizeInfo_(resizeInfo)
		{}

		virtual ILayout*			Clone() const		{ return new CFlowLayout(Direction_, Items_, ResizeInfo_); }
		EnumLayoutDirection			GetDirection()		{ return Direction_; }

		virtual const ResizeInfo&	GetResizeInfo(EnumLayoutDirection dir);
		virtual int					Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea);

		ILayout*					AddItem(const ILayout& item);

		CCtrlLayout*				AddCtrl(HWND hWnd, 
											ResizeInfo resizeInfoH = ResizeInfo::FixedInfo, 
											ResizeInfo resizeInfoV = ResizeInfo::FixedInfo, 
											AnchorInfo anchorInfoH = AnchorInfo::AnchorFront,
											AnchorInfo anchorInfoV = AnchorInfo::AnchorFront);

		CFlowLayout*				AddFlow(EnumLayoutDirection dir, ResizeInfo resizeInfo = ResizeInfo(EnumResizeMode::Auto, 0));
		
		CZoomBlankLayout*			AddZoomBlank(long percentH = 100, long percentV = 100);

	protected:
		virtual int					UpdateAutoSize();
		virtual long				GetFixedItemsLength();

	protected:
		EnumLayoutDirection			Direction_;
		ResizeInfo					ResizeInfo_;

		std::list<ILayout*>			Items_;
	};
}
